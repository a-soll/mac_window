#include "../util/windows.h"
#include <ApplicationServices/ApplicationServices.h>

// TODO: move AXUI logic. Window structs will only use CFArrayRef.
void _getUIElements(Window *w) {
    CFArrayRef windows;

    // create accessibility object by app's pid
    AXUIElementRef elem = AXUIElementCreateApplication(w->pid);

    // populate array of all UI element values for the windows
    AXError err = AXUIElementCopyAttributeValue(elem, kAXWindowsAttribute, (CFTypeRef *)&windows);
    w->uiElements = windows;
    if (windows) {
        if (CFArrayGetCount(windows) > 0) {
            CFTypeRef pos;
            CFTypeRef size;

            // create UIElement for item in first spot of array
            AXUIElementRef windowRef = (AXUIElementRef)CFArrayGetValueAtIndex(windows, 0);

            // copies the position attribute to pos
            AXUIElementCopyAttributeValue(windowRef, kAXPositionAttribute, (CFTypeRef *)&pos);
            AXUIElementCopyAttributeValue(windowRef, kAXSizeAttribute, (CFTypeRef *)&size);
            AXValueGetValue(pos, kAXValueCGPointType, &w->position);
            AXValueGetValue(size, kAXValueCGSizeType, &w->size);
            CFRelease(pos);
            CFRelease(size);
        }
    }
    CFRelease(elem);
}

int getWindowList(Window **w) {
    int length = 0;
    Window *win;

    CFArrayRef ws = CGWindowListCreate(kCGWindowListOptionAll, kCGNullWindowID);
    CFIndex count = CFArrayGetCount(ws);
    CFArrayRef ws_info = CGWindowListCreateDescriptionFromArray(ws);

    // reallocate on consecutive calls
    if (*w == NULL) {
        win = malloc(count * sizeof(Window));
    } else {
        win = realloc(*w, count * sizeof(Window));
    }

    for (int i = 0; i < count; i++) {
        Window window;
        CGRect dimensions;
        const void *owner;
        const void *pid;
        const void *onScreen;
        int *windowLayer;

        CFDictionaryRef info = (CFTypeRef)CFArrayGetValueAtIndex(ws_info, i);
        CFStringRef bounds = CFDictionaryGetValue(info, kCGWindowBounds);
        windowLayer = (int *)CFDictionaryGetValue(info, kCGWindowLayer);
        owner = CFDictionaryGetValue(info, kCGWindowOwnerName);
        pid = CFDictionaryGetValue(info, kCGWindowOwnerPID);

        CFIndex len = CFStringGetLength(owner) + 1;
        int memUsage;
        char cName[len];

        // gets the position and size
        CGRectMakeWithDictionaryRepresentation((CFTypeRef)CFDictionaryGetValue(info, kCGWindowBounds), &dimensions);
        window.size = dimensions.size;
        window.position = dimensions.origin;
        CFStringGetCString(owner, cName, len, kCFStringEncodingUTF8);
        CFNumberGetValue(pid, kCFNumberNSIntegerType, &pid);
        window.pid = (int)pid;
        strcpy(window.name, cName);
        _getUIElements(&window);
        win[i] = window;
    }
    *w = win;
    CFRelease(ws_info);
    CFRelease(ws);
    return count;
}

int getWindowByName(Window *w, int count, const char *appName) {
    int ret = -1;

    for (int i = 0; i < count; i++) {
        if (strcmp(w[i].name, appName) == 0) {
            ret = i;
        }
    }
    return ret;
}

// TODO: make this work
void moveWindow(Window *w, int x, int y) {
    CGPoint newPosition = {x, y};
    CFTypeRef pos = (CFTypeRef)(AXValueCreate(kAXValueCGPointType, (const void *)&newPosition));
    CFShow(w->uiElements);
    // AXUIElementSetAttributeValue(w->uiElements, kAXPositionAttribute, pos);
}

void releaseWindow(Window *w, int count) {
    for (int i = 0; i < count; i++) {
        if (w[i].uiElements) {
            CFRelease(w[i].uiElements);
        }
    }
    free(w);
}
