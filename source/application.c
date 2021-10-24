#include "include/application.h"

extern Table *app_table;
extern Table *proc_table;
extern int g_connection;

// get path and CFURL of app
void getApplicationPath(Application *application) {
    char path_of_app[PROC_PIDPATHINFO_MAXSIZE];
    proc_pidpath(application->pid, path_of_app, sizeof(path_of_app));
    CFURLRef path_url = loadBundle(path_of_app);
    strcpy(application->path, path_of_app);
    application->urlRef = path_url;
}

uint32_t getApplicationFocusedWindow(Application *application) {
    uint32_t wid;
    CFTypeRef window_ref;
    AXUIElementCopyAttributeValue(application->uiElem, kAXFocusedWindowAttribute, &window_ref);
    _AXUIElementGetWindow(window_ref, &wid);
    if (window_ref != NULL) {
        CFRelease(window_ref);
    }
    return wid;
}

#pragma clang diagnostic ignored "-Wdeprecated-declarations"
Application *getActiveApplication() {
    Application *application;
    pid_t pid;
    ProcessSerialNumber psn;

    _SLPSGetFrontProcess(&psn);
    GetProcessPID(&psn, &pid);
    application = table_search(app_table, pid);
    return application;
}

bool appIsHidden(Application *application) {
    Boolean hidden;
    CFTypeRef tmp;

    AXUIElementCopyAttributeValue(application->uiElem, kAXHiddenAttribute, &tmp);
    hidden = CFBooleanGetValue(tmp);
    CFRelease(tmp);
    return hidden;
}

AXError applicationUnHide(Application *application) {
    AXError err;
    err = AXUIElementSetAttributeValue(application->uiElem, kAXHiddenAttribute, kCFBooleanFalse);
    return err;
}

// apparently hiding is unreliable
AXError applicationHide(Application *application) {
    AXError err;
    err = AXUIElementSetAttributeValue(application->uiElem, kAXHiddenAttribute, kCFBooleanTrue);
    if (!appIsHidden(application)) {
        applicationHide(application);
    }
    return err;
}

CFArrayRef getApplicationWindows(Application *application) {
    CFTypeRef window_list;

    AXUIElementCopyAttributeValue(application->uiElem, kAXWindowsAttribute, &window_list);
    if (window_list != NULL) {
        application->windowCount = CFArrayGetCount(window_list);
    }
    return window_list;
}

void initApplicationList() {
    for (int i = 0; i < proc_table->size; i++) {
        if (valid_bucket(proc_table, i)) {
            Process *process = (Process *)proc_table->buckets[i]->data;
            Application *application = malloc(sizeof(Application));
            strcpy(application->name, process->name);
            application->pid = process->pid;
            application->psn = process->psn;
            application->uiElem = AXUIElementCreateApplication(application->pid);
            getApplicationPath(application);
            table_insert(app_table, application->pid, application);
            app_table->release = &releaseApplication;
        }
    }
}

int getApplicationByName(Application *a, const char *name, int length) {
    int ind = -1;

    for (int i = 0; i < length; i++) {
        if (strcmp(name, a[i].name) == 0) {
            ind = i;
            break;
        }
    }
    return ind;
}

void releaseApplication(void *application) {
    Application *app = (Application *)application;
    CFRelease(app->uiElem);
    CFRelease(app->urlRef);
}

OSStatus launchApplication(Application *application) {
    OSStatus did_launch;
    did_launch = LSOpenCFURLRef(application->urlRef, NULL);
    return did_launch;
}

#pragma clang diagnostic ignored "-Wdeprecated-declarations"
bool applicationIsHidden(Application *application) {
    bool hidden = IsProcessVisible(&application->psn);
    return hidden;
}
