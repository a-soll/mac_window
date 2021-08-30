#include "include/application.h"

extern Table *app_table;

CFArrayRef getApplicationWindows(Application *application) {
    CFTypeRef window_list;
    AXUIElementCopyAttributeValue(application->uiElem, kAXWindowsAttribute, &window_list);
    return window_list;
}

Application *initApplication(Process *process) {
    Application *application = malloc(sizeof(Application));
    application->pid = process->pid;
    application->uiElem = AXUIElementCreateApplication(application->pid);
    strcpy(application->name, process->name);
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
}
