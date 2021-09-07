#include "include/space.h"

extern int g_connection;
extern Table *space_table;
extern Table *display_table;

void spaceWindows(uint64_t sid) {
    
}

void initSpaceList() {
    int ret;
    uint64_t *sid_list;
    for (int i = 0; i < display_table->size; i++) {
        if (valid_bucket(display_table, i)) {
            Display *display = display_table->buckets[i]->data;
            sid_list = spaceListForDisplay(display->did);
        }
    }
    for (int i = 0; i < 4; i++) {
        printf("%llu\n", sid_list[i]);
    }
    free(sid_list);
}
