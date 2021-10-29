#include "include/application.h"

extern Table *app_table;
extern Table *proc_table;
extern int g_connection;

// get path and CFURL of app
void getApplicationPath(Application *application) {
    char path_of_app[PROC_PIDPATHINFO_MAXSIZE];
    proc_pidpath(application->pid, path_of_app, sizeof(path_of_app));
    CFURLRef path_url = load_bundle(path_of_app);
    strcpy(application->path, path_of_app);
    application->urlRef = path_url;
}

uint32_t get_application_focused_window(Application *application) {
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
Application *get_active_application() {
    Application *application;
    pid_t pid;
    ProcessSerialNumber psn;

    _SLPSGetFrontProcess(&psn);
    GetProcessPID(&psn, &pid);
    application = table_search(app_table, pid);
    return application;
}

bool app_is_hidden(Application *application) {
    Boolean hidden;
    CFTypeRef tmp;

    AXUIElementCopyAttributeValue(application->uiElem, kAXHiddenAttribute, &tmp);
    hidden = CFBooleanGetValue(tmp);
    CFRelease(tmp);
    return hidden;
}

AXError application_unhide(Application *application) {
    AXError err;
    err = AXUIElementSetAttributeValue(application->uiElem, kAXHiddenAttribute, kCFBooleanFalse);
    return err;
}

// apparently hiding is unreliable
AXError application_hide(Application *application) {
    AXError err;
    err = AXUIElementSetAttributeValue(application->uiElem, kAXHiddenAttribute, kCFBooleanTrue);
    if (!app_is_hidden(application)) {
        application_hide(application);
    }
    return err;
}

CFArrayRef get_application_windows(Application *application) {
    CFTypeRef window_list;

    AXUIElementCopyAttributeValue(application->uiElem, kAXWindowsAttribute, &window_list);
    if (window_list != NULL) {
        application->windowCount = CFArrayGetCount(window_list);
    }
    return window_list;
}

AXError application_observe(Application *application, AXObserverCallback callback) {
    AXError err = AXObserverCreate(application->pid, callback, &application->observer);
    return err;
}

void init_application_list() {
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
            app_table->release = &release_application;
        }
    }
}

int get_application_by_name(Application *a, const char *name, int length) {
    int ind = -1;

    for (int i = 0; i < length; i++) {
        if (strcmp(name, a[i].name) == 0) {
            ind = i;
            break;
        }
    }
    return ind;
}

void release_application(void *application) {
    Application *app = (Application *)application;
    CFRelease(app->uiElem);
    CFRelease(app->urlRef);
}

OSStatus launch_application(Application *application) {
    OSStatus did_launch;
    did_launch = LSOpenCFURLRef(application->urlRef, NULL);
    return did_launch;
}

#pragma clang diagnostic ignored "-Wdeprecated-declarations"
bool application_is_hidden(Application *application) {
    bool hidden = IsProcessVisible(&application->psn);
    return hidden;
}
