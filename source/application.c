#include "include/application.h"

extern Table *app_table;

// get path and CFURL of app
void getApplicationPath(Application *application) {
    char path_of_app[PROC_PIDPATHINFO_MAXSIZE];
    proc_pidpath(application->pid, path_of_app, sizeof(path_of_app));
    CFURLRef path_url = loadBundle(path_of_app);
    strcpy(application->path, path_of_app);
    application->urlRef = path_url;
}

CFArrayRef getApplicationWindows(Application *application) {
    CFTypeRef window_list;
    AXUIElementCopyAttributeValue(application->uiElem, kAXWindowsAttribute, &window_list);
    if (window_list != NULL) {
        application->windowCount = CFArrayGetCount(window_list);
    }
    return window_list;
}

Application *initApplication(Process *process) {
    Application *application = malloc(sizeof(Application));
    application->psn = process->psn;
    application->pid = process->pid;
    application->uiElem = AXUIElementCreateApplication(application->pid);
    strcpy(application->name, process->name);
    getApplicationPath(application);
    table_insert(app_table, application->pid, application);
    app_table->release = &releaseApplication;
    return application;
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
