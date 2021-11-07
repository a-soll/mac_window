#include "include/space.h"

extern int g_connection;
extern Table *space_table;
extern Table *display_table;

Space *get_space(uint64_t sid) {
    Space *space;
    space = (Space *)table_search(space_table, sid);
    return space;
}

/* pass display uuid. `killall Dock` needs to be called for
 * space to become visible.
 */
Space *create_space(CFStringRef uuid) {
    Space *space = malloc(sizeof(Space));

    CFTypeRef keys[2];
    keys[0] = CFSTR("type");
    keys[1] = CFSTR("uuid");

    CFTypeRef values[2];
    int type_int = CGSSpaceTypeUser;
    CFNumberRef type = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &type_int);
    values[0] = type;
    values[1] = uuid;

    CFDictionaryRef options = CFDictionaryCreate(kCFAllocatorDefault, (const void **)&keys, (const void **)&values, 2, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    space->sid = CGSSpaceCreate(g_connection, (void *)uuid, options);
    table_insert(space_table, space->sid, space);

    CFRelease(options);
    return space;
}

uint64_t get_active_space() {
    Application *application = get_active_application();
    uint64_t wid = get_application_focused_window(application);
    CFStringRef duuid = get_display_for_window_id(wid);
    uint64_t sid = SLSManagedDisplayGetCurrentSpace(g_connection, duuid);
    CFRelease(duuid);
    return sid;
}

CFArrayRef space_windows(uint64_t sid) {
    uint64_t set_tags = 0;
    uint64_t clear_tags = 0;
    CFArrayRef space_list_ref = cfarray_from_numbers(&sid, sizeof(uint64_t), 1, kCFNumberSInt32Type);
    CFArrayRef window_list_ref = SLSCopyWindowsWithOptionsAndTags(g_connection, 0, space_list_ref, 0x2, &set_tags, &clear_tags);
    CFRelease(space_list_ref);
    return window_list_ref;
}

void init_space_list() {
    space_table->release = NULL;
    int ret;
    int space_count;
    uint64_t *sid_list;
    Display *display = (Display *)table_iterate(display_table, true);
    do {
        space_count = space_list_for_display(display->did, &sid_list);

        for (int i = 0; i < space_count; i++) {
            Space *space;
            space = malloc(sizeof(Space));
            space->sid = sid_list[i];
            table_insert(space_table, space->sid, space);
        }
    } while ((display = (Display *)table_iterate(display_table, false)));
    free(sid_list);
}
