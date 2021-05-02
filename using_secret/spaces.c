#include "CGSSpaces.h"
#include <ApplicationServices/ApplicationServices.h>
#include <CoreGraphics/CoreGraphics.h>
#include <NSObjects/NSBase.h>

// get number of spaces
int spaces_count(int cid) {
    int ret;
    uint32_t did = CGMainDisplayID();
    uint64_t set_tags = 0;
    uint64_t clear_tags = 0;
    uint64_t options = 0x7;

    CFUUIDRef uuid_ref = CGDisplayCreateUUIDFromDisplayID(did);
    CFStringRef uuid_str = CFUUIDCreateString(NULL, uuid_ref);
    // array of all displays containing an array of spaces for that display
    CFArrayRef spaces = SLSCopyManagedDisplaySpaces(cid);
    int space_count = CFArrayGetCount(spaces);

    for (int i = 0; i <= space_count; i++) {
        CFDictionaryRef display_ref = CFArrayGetValueAtIndex(spaces, i);
        CFStringRef identifier = CFDictionaryGetValue(display_ref, CFSTR("Display Identifier"));

        if (!CFEqual(uuid_str, identifier)) {
            continue;
        }

        CFArrayRef spaces_ref = CFDictionaryGetValue(display_ref, CFSTR("Spaces"));
        ret = CFArrayGetCount(spaces_ref);
        break;
    }
    return ret;
}

int main() {
    // cid
    int g_connection = SLSMainConnectionID();
    int num_spaces;

    num_spaces = spaces_count(g_connection);
    printf("%d\n", num_spaces);
}
