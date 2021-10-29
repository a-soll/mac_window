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

    // displayCount = get_display_list(&displayList);
    void *g_workspace_context;

    workspace_event_handler_init(&g_workspace_context);
    workspace_event_handler_begin(&g_workspace_context);

    display_table = table_init(5);
    space_table = table_init(5);
    proc_table = table_init(125);
    app_table = table_init(125);
    window_table = table_init(20);

    get_display_list();
    init_space_list();
    get_process_list();
    init_application_list();
    get_window_list();

    for (int i = 0; i < window_table->size; i++) {
        if (valid_bucket(window_table, i)) {
            Window *window = window_table->buckets[i]->data;
            uint64_t sid = current_space_for_window(window);
            printf("WID: %d\n", window->wid);
            printf("Name: %s\n", window->application->name);
            CFShow(window->uiElem);
            printf("Height: %f\n", window->size.height);
            printf("SID: %llu\n", sid);
            printf("Mini? %d\n", window->isMinimized);
            printf("\n");
        }
    }

    bridgeNSAppLoad();
    // CFRunLoopRun();
    table_free(proc_table);
    table_free(app_table);
    table_free(window_table);
    table_free(display_table);
}
