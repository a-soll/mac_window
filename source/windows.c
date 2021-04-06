#include "../util/windows.h"

int getWindowList(Window **w) {
    int length = 0;
    Window *win;
    CFArrayRef ws = CGWindowListCreate(kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
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
        const void *memory;

        CFDictionaryRef info = (CFTypeRef)CFArrayGetValueAtIndex(ws_info, i);
        CFStringRef bounds = CFDictionaryGetValue(info, kCGWindowBounds);
        owner = CFDictionaryGetValue(info, kCGWindowOwnerName);
        pid = CFDictionaryGetValue(info, kCGWindowOwnerPID);
        onScreen = CFDictionaryGetValue(info, kCGWindowIsOnscreen);
        memory = CFDictionaryGetValue(info, kCGWindowMemoryUsage);

        CFIndex len = CFStringGetLength(owner) + 1;
        int memUsage;
        char cName[len];

        // gets the position and size
        CGRectMakeWithDictionaryRepresentation((CFTypeRef)CFDictionaryGetValue(info, kCGWindowBounds), &dimensions);
        window.size = dimensions.size;
        window.position = dimensions.origin;
        CFNumberGetValue(memory, kCFNumberNSIntegerType, &memory);
        CFStringGetCString(owner, cName, len, kCFStringEncodingUTF8);
        CFNumberGetValue(pid, kCFNumberNSIntegerType, &pid);
        window.pid = (int)pid;
        strcpy(window.name, cName);
        window.memoryUsage = (int)memory;
        win[i] = window;
    }

    *w = win;
    CFRelease(ws_info);
    CFRelease(ws);
    return count;
}
