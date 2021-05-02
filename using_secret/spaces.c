#include "CGSSpaces.h"
#include <ApplicationServices/ApplicationServices.h>
#include <CoreGraphics/CoreGraphics.h>
#include <NSObjects/NSBase.h>

// cid = connection ID
// sid = space ID
// count = number of spaces
uint32_t *window_list_for_spaces(int cid, uint64_t sid) {
    ProcessSerialNumber front_psn;
    uint32_t *window_list = NULL;
    uint64_t set_tags = 0;
    uint64_t clear_tags = 0;
    int count;
    int front_cid;

    _SLPSGetFrontProcess(&front_psn);
    SLSGetConnectionIDForPSN(cid, &front_psn, &front_cid);

    CFNumberRef space_id_ref = CFNumberCreate(NULL, kCFNumberSInt32Type, &sid);
    CFArrayRef space_list_ref = CFArrayCreate(NULL, (void *)&space_id_ref, 1, NULL);

    CFArrayRef window_list_ref = SLSCopyWindowsWithOptionsAndTags(cid, front_cid, space_list_ref, 0x2, &set_tags, &clear_tags);
    count = CFArrayGetCount(window_list_ref);
    CFShow(window_list_ref);

    for (int i = 0; i < count; ++i) {
        CFNumberRef id_ref = CFArrayGetValueAtIndex(window_list_ref, i);
        CFShow(id_ref);
        // CFNumberGetValue(id_ref, CFNumberGetType(id_ref), (void*)window_list + i);
    }
    // CFArrayRef space_list_ref = SLSCopyWindowsWithOptionsAndTags(g_connection, 0? (might be owner from call), CFArrayRef spaces, uint32_t options, uint64_t *set_tags, uint64_t *clear_tags);
    return window_list;
}

// get number of spaces and populate array of sids
int get_sids(int cid, uint32_t did, uint64_t **sid_list) {
    int ret;

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

        // populate list of space IDs
        *sid_list = malloc(ret * sizeof(uint64_t));
        for (int i = 0; i < ret; i++) {
            CFNumberRef temp_sid;
            CFDictionaryRef space_at_ind = CFArrayGetValueAtIndex(spaces_ref, i);
            temp_sid = (CFNumberRef)CFDictionaryGetValue(space_at_ind, CFSTR("ManagedSpaceID"));
            CFNumberGetValue(temp_sid, kCFNumberSInt64Type, (void *)&(*sid_list)[i]);
        }
        break;
    }
    return ret;
}

int main() {
    // cid
    int g_connection = SLSMainConnectionID();
    uint32_t did = CGMainDisplayID();
    CFArrayRef spaces_ref;
    int num_spaces;
    int num;
    uint64_t *sid_list;

    num = get_sids(g_connection, did, &sid_list);

    for (int i = 0; i < num; i++) {
        uint32_t *window_list = window_list_for_spaces(g_connection, sid_list[i]);
    }
}
