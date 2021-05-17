#include "include/applications.h"
#include "include/includes.h"
#include <string.h>

bool duplicatePid(Application *app, pid_t pid, int length) {
    bool duplicate = false;
    for (int i = 0; i < length; i++) {
        if (app[i].pid == pid) {
            duplicate = true;
        }
    }
    return duplicate;
}

// returns true if the app has window AXUIElements
bool addApplicationWindows(Window **w, pid_t pid, int *windowCount) {
    Window *window;
    bool hasWindows = false;
    CFArrayRef uiElems;
    int count;
    AXUIElementRef elem = AXUIElementCreateApplication(pid);
    AXError err = AXUIElementCopyAttributeValue(elem, kAXWindowsAttribute, (CFTypeRef *)&uiElems);

    if (err == kAXErrorSuccess) {
        count = CFArrayGetCount(uiElems);
        if (count > 0) {
            // reallocate on consecutive calls
            if (*w == NULL) {
                window = malloc(count * sizeof(Window));
            } else {
                window = realloc(*w, count * sizeof(Window));
            }
            *windowCount = count;
            initWindow(window, uiElems, count);
            hasWindows = true;
        }
    }
    *w = window;
    CFRelease(elem);
    if (uiElems) {
        CFRelease(uiElems);
    }
    return hasWindows;
}

int getOpenApplications(Application **a) {
    int length = 0;
    Application *app;
    pid_t last_pid = -1;
    CFArrayRef ws = CGWindowListCreate(kCGWindowListOptionOnScreenOnly | kCGWindowListExcludeDesktopElements, kCGNullWindowID);
    CFIndex count = CFArrayGetCount(ws);
    CFArrayRef ws_info = CGWindowListCreateDescriptionFromArray(ws);

    // reallocate on consecutive calls
    if (*a == NULL) {
        app = malloc(count * sizeof(Application));
    } else {
        app = realloc(*a, count * sizeof(Application));
    }

    for (int i = 0; i < count; i++) {
        Application application;
        application.windows = NULL;
        CFNumberRef pid;
        pid_t temp_pid;
        CFStringRef owner;

        CFDictionaryRef info = (CFTypeRef)CFArrayGetValueAtIndex(ws_info, i);
        pid = (CFNumberRef)CFDictionaryGetValue(info, kCGWindowOwnerPID);
        CFNumberGetValue(pid, kCFNumberNSIntegerType, (void *)&temp_pid);
        owner = (CFStringRef)CFDictionaryGetValue(info, kCGWindowOwnerName);
        CFIndex len = CFStringGetLength(owner) + 1;
        if (!duplicatePid(app, temp_pid, length)) {
            char cName[len];
            CFStringGetCString(owner, cName, len, kCFStringEncodingUTF8);
            application.pid = temp_pid;
            strcpy(application.name, cName);
            if (addApplicationWindows(&application.windows, temp_pid, &application.windowCount)) {
                app[length] = application;
                length++;
            }
            last_pid = temp_pid;
        }
    }
    CFRelease(ws_info);
    CFRelease(ws);
    *a = app;
    return length;
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

void releaseApplicationList(Application *a, int count) {
    for (int i = 0; i < count; i++) {
        releaseWindowList(a[i].windows, a[i].windowCount);
    }
    free(a);
}
