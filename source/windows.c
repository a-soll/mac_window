#include "../util/windows.h"
#include <ApplicationServices/ApplicationServices.h>

void _getUIElements(Window *w) {
    CFArrayRef windows;
    AXUIElementRef elem = AXUIElementCreateApplication(w->pid);

    // populate array of all UI element values for the app's windows
    AXError err = AXUIElementCopyAttributeValue(elem, kAXWindowsAttribute, (CFTypeRef *)&windows);
    if (err == kAXErrorSuccess) {
        w->uiElements = windows;
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
        CFStringRef owner;
        CFNumberRef pid;
        CFNumberRef windowLayer;

        CFDictionaryRef info = (CFTypeRef)CFArrayGetValueAtIndex(ws_info, i);
        CFStringRef bounds = CFDictionaryGetValue(info, kCGWindowBounds);
        windowLayer = (CFNumberRef)CFDictionaryGetValue(info, kCGWindowLayer);
        owner = (CFStringRef)CFDictionaryGetValue(info, kCGWindowOwnerName);
        pid = (CFNumberRef)CFDictionaryGetValue(info, kCGWindowOwnerPID);

        CFIndex len = CFStringGetLength(owner) + 1;
        char cName[len];

        // gets the position and size
        CGRectMakeWithDictionaryRepresentation((CFTypeRef)CFDictionaryGetValue(info, kCGWindowBounds), &dimensions);
        window.size = dimensions.size;
        window.position = dimensions.origin;
        CFStringGetCString(owner, cName, len, kCFStringEncodingUTF8);

        CFNumberGetValue(pid, kCFNumberNSIntegerType, (void *)&window.pid);
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

void moveWindow(Window *w, CGPoint p) {
    AXUIElementRef uiwindow;
    AXError moved;
    CFTypeRef pos = (CFTypeRef)(AXValueCreate(kAXValueCGPointType, (const void *)&p));
    int len = CFArrayGetCount(w->uiElements);

    for (int i = 0; i < len; i++) {
        uiwindow = CFArrayGetValueAtIndex(w->uiElements, i);
        moved = AXUIElementSetAttributeValue(uiwindow, kAXPositionAttribute, pos);
    }
    if (moved == kAXErrorSuccess) {
        w->position = p;
    }
    CFRelease(pos);
}

void releaseWindow(Window *w, int count) {
    for (int i = 0; i < count; i++) {
        if (w[i].uiElements) {
            CFRelease(w[i].uiElements);
        }
    }
    free(w);
}
