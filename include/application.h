#ifndef APPLICATIONS_H
#define APPLICATIONS_H

#include "base.h"
#include "process.h"
#include "bundle.h"

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

CFArrayRef getApplicationWindows(Application *application);
void initApplicationList();
int getOpenApplications(Application **app);
void applicationGetWindowList(Application **app);
void releaseApplication(void *application);
void releaseApplicationList(Application *a, int count);
bool applicationIsHidden(Application *application);
// returns index of the app
int getApplicationByName(Application *a, const char *name, int length);
OSStatus launchApplication(Application *application);
Application *getActiveApplication();
// returns wid of focused window
uint32_t getApplicationFocusedWindow(Application *application);
bool appIsHidden(Application *application);
AXError applicationHide(Application *application);
AXError applicationUnHide(Application *application);

#endif /* APPLICATIONS_H */
