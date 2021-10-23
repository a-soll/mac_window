#ifndef APPLICATIONS_H
#define APPLICATIONS_H

#include "base.h"
#include "process.h"
#include "bundle.h"

/**Application attributes
 * pid_t pid = pid of app
 * char name[99] = name of app
 * int windowCount = number of windows in Window
 * Window windows = Window structs for valid open windows of the app
 */
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

Application *initApplication(Process *process);

int getOpenApplications(Application **app);

void applicationGetWindowList(Application **app);

void releaseApplication(void *application);

void releaseApplicationList(Application *a, int count);

bool applicationIsHidden(Application *application);

// returns index of the app
int getApplicationByName(Application *a, const char *name, int length);

OSStatus launchApplication(Application *application);

#endif /* APPLICATIONS_H */
