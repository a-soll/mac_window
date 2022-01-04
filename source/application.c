#include "include/application.h"

extern Table *app_table;
extern Table *proc_table;
extern int g_connection;

// get path and CFURL of app
void get_application_path(Application *application) {
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
    application = get_application(pid);
    return application;
}

Application *get_application(pid_t pid) {
    Application *application;
    application = (Application *)table_search(app_table, pid);
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
    return window_list;
}

AXError application_observe(Application *application, AXObserverCallback callback) {
    AXError err = AXObserverCreate(application->pid, callback, &application->observer);
    return err;
}

void init_application_list() {
    Process *process = (Process *)table_iterate(proc_table, true);
    do {
        Application *application = malloc(sizeof(Application));
        strcpy(application->name, process->name);
        application->windowCount = -1;
        application->wids = NULL;
        application->pid = process->pid;
        application->psn = process->psn;
        application->uiElem = AXUIElementCreateApplication(application->pid);
        get_application_path(application);
        table_insert(app_table, application->pid, application);
        app_table->release = &release_application;
    } while ((process = table_iterate(proc_table, false)));
}

void release_application(void *application) {
    Application *app = (Application *)application;
    free(app->wids);
    CFRelease(app->uiElem);
    CFRelease(app->urlRef);
}

OSStatus launch_application(char *path) {
    OSStatus did_launch;
    CFURLRef url = load_bundle(path);
    CFShow(url);
    did_launch = LSOpenCFURLRef(url, NULL);
    CFRelease(url);
    return did_launch;
}

#pragma clang diagnostic ignored "-Wdeprecated-declarations"
bool application_is_hidden(Application *application) {
    bool hidden = IsProcessVisible(&application->psn);
    return hidden;
}
