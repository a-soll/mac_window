#include "include/base.h"
#include "include/includes.h"
#include "include/notif.h"

#define kCPSUserGenerated 0x200

int g_connection;
Table *proc_table;
Table *app_table;
Table *window_table;
Table *display_table;
Table *space_table;

int main() {
    g_connection = SLSMainConnectionID();

    // displayCount = getDisplayList(&displayList);
    void *g_workspace_context;

    workspace_event_handler_init(&g_workspace_context);
    workspace_event_handler_begin(&g_workspace_context);
    proc_table = table_init(125);
    app_table = table_init(125);
    window_table = table_init(125);
    display_table = table_init(5);
    space_table = table_init(5);

    getProcessList();
    getWindowList();
    getDisplayList();
    initSpaceList();

    for (int i = 0; i < display_table->size; i++) {
        if (valid_bucket(display_table, i)) {
            Display *dis = display_table->buckets[i]->data;
            // printf("%d\n", dis->did);
        }
    }

    for (int i = 0; i < window_table->size; i++) {
        if (valid_bucket(window_table, i)) {
            Window *window = window_table->buckets[i]->data;
            // printf("WID: %d\n", window->wid);
        }
    }

    CFArrayRef window_list = CGWindowListCopyWindowInfo(kCGWindowListExcludeDesktopElements, 0);
    int n = CFArrayGetCount(window_list);
    for (int i = 0; i < n; i++) {
        CFDictionaryRef info = CFArrayGetValueAtIndex(window_list, i);
        CFNumberRef store_type = CFDictionaryGetValue(info, kCGWindowStoreType);
        int store;
        CFNumberGetValue(store_type, CFNumberGetType(store_type), &store);
        // if (CFStringCompare(CFDictionaryGetValue(info, kCGWindowOwnerName), CFSTR("Code"), 0) == 0 && store == 1) {
        //     CFShow(info);
        // }
    }

    // SLSProcessAssignToSpace(g_connection, 86303, 5);
    // SLSProcessAssignToAllSpaces(g_connection, 86303);

    
    bridgeNSAppLoad();
    // CFRunLoopRun();
    table_free(proc_table);
    table_free(app_table);
    table_free(window_table);
    table_free(display_table);
}
