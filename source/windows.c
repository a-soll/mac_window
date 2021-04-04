#include "../util/windows.h"

// return the index of specified window in windowList
void getWindowByName(Window *w, const char *name) {
    int length = 0;
    // CFArrayRef ws = CGWindowListCreate(kCGWindowListOptionAll, kCGNullWindowID);
    CFArrayRef ws = CGWindowListCreate(kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
    CFIndex count = CFArrayGetCount(ws);
    CFArrayRef ws_info = CGWindowListCreateDescriptionFromArray(ws);

    for (int i = 0; i < count; i++) {
        const void *owner;
        const void *pid;
        const void *onScreen;
        const void *memory;

        CFStringRef str = CFStringCreateWithCString(kCFAllocatorDefault, name, kCFStringEncodingUTF8);
        CFDictionaryRef info = (CFTypeRef)CFArrayGetValueAtIndex(ws_info, i);
        CFStringRef bounds = CFDictionaryGetValue(info, kCGWindowBounds);
        owner = CFDictionaryGetValue(info, kCGWindowOwnerName);
        pid = CFDictionaryGetValue(info, kCGWindowOwnerPID);
        onScreen = CFDictionaryGetValue(info, kCGWindowIsOnscreen);
        memory = CFDictionaryGetValue(info, kCGWindowMemoryUsage);

        if (owner && pid) {
            if (CFStringCompare(owner, str, 0) == kCFCompareEqualTo && onScreen) {
                CFIndex len = CFStringGetLength(owner) + 1;
                int memUsage;
                char cName[len];

                if (onScreen) {
                    w->OnScren = true;
                } else {
                    w->OnScren = false;
                }
                CFNumberGetValue(memory, kCFNumberNSIntegerType, &memory);
                CFStringGetCString(owner, cName, len, kCFStringEncodingUTF8);
                CFNumberGetValue(pid, kCFNumberNSIntegerType, &pid);
                w->pid = (int)pid;
                w->name = cName;
                w->memoryUsage = (int)memory;
            }
        }
    }
}
