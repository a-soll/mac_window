#include "CGSSpaces.h"
#include <ApplicationServices/ApplicationServices.h>
#include <CoreGraphics/CoreGraphics.h>
#include <NSObjects/NSBase.h>

int main() {
    // cid
    int g_connection = SLSMainConnectionID();
    CFTypeRef pos;
    // CFTypeRef size;
    CFArrayRef windows;
    pid_t pid = 455;
    uint32_t wid;
    int dock;
    ProcessSerialNumber psn;
    CFTypeRef region;
    int g_floating_window_level = CGWindowLevelForKey(kCGFloatingWindowLevelKey);

    CGPoint origin = {0,0};

    CGSize size = {100,100};

    CGRect rect = {origin,size};

    //get spaces and windows
    uint32_t did = CGMainDisplayID();
    uint64_t set_tags = 0;
    uint64_t clear_tags = 0;
    uint64_t options = 0x7;

    CFUUIDRef uuid_ref = CGDisplayCreateUUIDFromDisplayID(did);
    CFStringRef uuid_str = CFUUIDCreateString(NULL, uuid_ref);
    CFArrayRef spaces = SLSCopyManagedDisplaySpaces(g_connection);
    int space_count;

    for (int i = 0; i < space_count; i++) {
        CFDictionaryRef display_ref = CFArrayGetValueAtIndex(spaces, i);
        CFStringRef identifier = CFDictionaryGetValue(display_ref, CFSTR("Display Identifier"));

        if (!CFEqual(uuid_str, identifier)) {
            continue;
        }

        CFArrayRef spaces_ref = CFDictionaryGetValue(display_ref, CFSTR("Spaces"));
        space_count = CFArrayGetCount(spaces_ref);
    }

    // -----------------------------------------------------
    AXUIElementRef elem = AXUIElementCreateApplication(pid);
    AXUIElementCopyAttributeValue(elem, kAXWindowsAttribute, (CFTypeRef *)&windows);
    AXUIElementRef windowRef = (AXUIElementRef)CFArrayGetValueAtIndex(windows, 0);

    _AXUIElementGetWindow(windowRef, &wid);

    CFStringRef uuid = SLSCopyActiveMenuBarDisplayIdentifier(g_connection);

    SLSGetWindowOwner(g_connection, wid, &dock);
    SLSGetConnectionPSN(dock, &psn);

    uint32_t space = SLSManagedDisplayGetCurrentSpace(g_connection, uuid);

    CGSNewRegionWithRect(&rect, &region);

    // printf("%d\n", space);
}
