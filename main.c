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

    display_table = table_init(5);
    space_table = table_init(5);
    proc_table = table_init(125);
    app_table = table_init(125);
    window_table = table_init(125);

    getDisplayList();
    initSpaceList();
    getProcessList();
    initApplicationList();
    getWindowList();

    for (int i = 0; i < window_table->size; i++) {
        if (valid_bucket(window_table, i)) {
            CFArrayRef stuff;
            Window *window = window_table->buckets[i]->data;
            printf("WID: %d\n", window->wid);
            printf("Name: %s\n", window->application->name);
            CFShow(window->uiElem);
            uint64_t sid = currentSpaceForWindow(window);
            if (sid != getActiveSpace()){
                SLSProcessAssignToSpace(g_connection, window->application->pid, getActiveSpace());
            }
            printf("%llu\n", sid);
            printf("\n");
            // printf("Height: %f\n", window->size.height);
            // printf("Hidden? %d\n", applicationIsHidden(window->application));
            // printf("Mini? %d\n", window->isMinimized);
        }
    }

    bridgeNSAppLoad();
    // CFRunLoopRun();
    table_free(proc_table);
    table_free(app_table);
    table_free(window_table);
    table_free(display_table);
}
