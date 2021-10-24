#include "include/display.h"

extern int g_connection;
extern Table *display_table;

void getDisplayDimensions(Display *display, CGDirectDisplayID did) {
    display->did = did;
    display->width = (double)CGDisplayPixelsWide(display->did);
    display->height = (double)CGDisplayPixelsHigh(display->did);

    // get corner coords
    display->topleft.x = display->origin.x;
    display->topleft.y = display->origin.y;

    display->topright.x = display->width + display->origin.x;
    display->topright.y = display->origin.y;

    display->bottomleft.x = display->origin.x;
    display->bottomright.y = display->origin.y + display->height;

    display->bottomright.x = display->width + display->origin.x;
    display->bottomright.y = display->height + display->origin.y;
}

int spaceListForDisplay(int did, uint64_t **sid_list) {
    CFUUIDRef uuid_ref = CGDisplayCreateUUIDFromDisplayID(did);
    int count;
    CFStringRef uuid_str = CFUUIDCreateString(NULL, uuid_ref);
    // array of all displays containing an array of spaces for that display
    CFArrayRef spaces = SLSCopyManagedDisplaySpaces(g_connection);
    int space_count = CFArrayGetCount(spaces);

    for (int i = 0; i <= space_count; i++) {
        CFDictionaryRef display_ref = CFArrayGetValueAtIndex(spaces, i);
        CFStringRef identifier = CFDictionaryGetValue(display_ref, CFSTR("Display Identifier"));

        if (!CFEqual(uuid_str, identifier)) {
            continue;
        }

        CFArrayRef spaces_ref = CFDictionaryGetValue(display_ref, CFSTR("Spaces"));
        count = CFArrayGetCount(spaces_ref);
        // populate list of space IDs
        *sid_list = malloc(count * sizeof(uint64_t));
        for (int i = 0; i < count; i++) {
            CFNumberRef temp_sid;
            CFDictionaryRef space_at_ind = CFArrayGetValueAtIndex(spaces_ref, i);
            temp_sid = (CFNumberRef)CFDictionaryGetValue(space_at_ind, CFSTR("ManagedSpaceID"));
            CFNumberGetValue(temp_sid, kCFNumberSInt64Type, (void *)&(*sid_list)[i]);
            CFRelease(temp_sid);
        }
        break;
    }
    CFRelease(spaces);
    CFRelease(uuid_ref);
    CFRelease(uuid_str);
    return count;
}

CFStringRef getDisplayForWindowId(uint64_t wid) {
    CFStringRef duuid;
    duuid = SLSCopyManagedDisplayForWindow(g_connection, wid);
    return duuid;
}

void getDisplayList() {
    display_table->release = &releaseDisplay;
    CFArrayRef display_list = SLSCopyManagedDisplays(g_connection);
    uint32_t count = CFArrayGetCount(display_list);
    CGDirectDisplayID ids[count];
    CGGetActiveDisplayList(count, ids, &count);

    for (int i = 0; i < count; i++) {
        Display *display;
        display = malloc(sizeof(Display));
        CGDirectDisplayID curId = ids[i];
        CGRect origin = CGDisplayBounds(curId);
        display->origin = origin.origin;
        if (curId == CGMainDisplayID()) {
            display->isMain = true;
        } else {
            display->isMain = false;
        }
        display->uuid = CFArrayGetValueAtIndex(display_list, i);
        getDisplayDimensions(display, curId);
        CFRetain(display->uuid);
        table_insert(display_table, display->did, (void *)display);
    }
    CFRelease(display_list);
}

void releaseDisplay(void *display) {
    Display *dis = (Display *)display;
    CFRelease(dis->uuid);
}
