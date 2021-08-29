#ifndef APPLICATIONS_H
#define APPLICATIONS_H

#include "base.h"

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
    int windowCount;
} Application;

CFArrayRef getApplicationWindows(Application app);

Application *initApplication();

int getOpenApplications(Application **app);

void applicationGetWindowList(Application **app);

void releaseApplication(Application *a, int index);

void releaseApplicationList(Application *a, int count);

// returns index of the app
int getApplicationByName(Application *a, const char *name, int length);

#endif /* APPLICATIONS_H */
