#include "include/space.h"

extern int g_connection;
extern Table *space_table;
extern Table *display_table;

Space *getSpace(uint64_t sid) {
    Space *space;
    space = table_search(space_table, sid);
    return space;
}

uint64_t getActiveSpace() {
    Application *application = getActiveApplication();
    uint64_t wid = getApplicationFocusedWindow(application);
    CFStringRef duuid = getDisplayForWindowId(wid);
    uint64_t sid = SLSManagedDisplayGetCurrentSpace(g_connection, duuid);
    CFRelease(duuid);
    return sid;
}

CFArrayRef spaceWindows(uint64_t sid) {
    uint64_t set_tags = 0;
    uint64_t clear_tags = 0;
    CFArrayRef space_list_ref = CFArrayFromNumbers(&sid, sizeof(uint64_t), 1, kCFNumberSInt32Type);
    CFArrayRef window_list_ref = SLSCopyWindowsWithOptionsAndTags(g_connection, 0, space_list_ref, 0x2, &set_tags, &clear_tags);
    CFRelease(space_list_ref);
    return window_list_ref;
}

void initSpaceList() {
    space_table->release = NULL;
    int ret;
    int space_count;
    uint64_t *sid_list;
    for (int i = 0; i < display_table->size; i++) {
        if (valid_bucket(display_table, i)) {
            Display *display = display_table->buckets[i]->data;
            space_count = spaceListForDisplay(display->did, &sid_list);
        }
    }

    for (int i = 0; i < space_count; i++) {
        Space *space;
        space = malloc(sizeof(Space));
        space->sid = sid_list[i];
        table_insert(space_table, space->sid, space);
    }
    free(sid_list);
}
