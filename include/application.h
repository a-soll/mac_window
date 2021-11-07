#ifndef APPLICATIONS_H
#define APPLICATIONS_H

#include "base.h"
#include "bundle.h"
#include "process.h"

typedef struct Application {
    AXUIElementRef uiElem;
    pid_t pid;
    char name[APP_NAME_MAX];
    char path[PROC_PIDPATHINFO_MAXSIZE];
    CFURLRef urlRef;
    int windowCount;
    AXObserverRef observer;
    ProcessSerialNumber psn;
} Application;

CFArrayRef get_application_windows(Application *application);
void init_application_list();
int get_open_applications(Application **app);
void application_get_window_list(Application **app);
void release_application(void *application);
void release_application_list(Application *a, int count);
bool application_is_hidden(Application *application);
// returns index of the app
int get_application_by_name(Application *a, const char *name, int length);
OSStatus launch_application(Application *application);
Application *get_active_application();
// returns wid of focused window
uint32_t get_application_focused_window(Application *application);
bool app_is_hidden(Application *application);
AXError application_hide(Application *application);
AXError application_unhide(Application *application);
AXError application_observe(Application *application, AXObserverCallback callback);
Application *get_application(pid_t pid);

#endif /* APPLICATIONS_H */
