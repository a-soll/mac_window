#include "include/base.h"
#include "include/includes.h"
#include "include/notif.h"

int g_connection;
Table *proc_table;
Table *app_table;
Table *window_table;

int main() {
    g_connection = SLSMainConnectionID();
    displayList = NULL;

    displayCount = getDisplayList(&displayList);
    void *g_workspace_context;

    workspace_event_handler_init(&g_workspace_context);
    workspace_event_handler_begin(&g_workspace_context);
    proc_table = table_init(125);
    app_table = table_init(125);
    window_table = table_init(125);

    getProcessList();
    getWindowList();

    table_free(proc_table);
    table_free(app_table);
    table_free(window_table);
    free(displayList);
    // bridgeNSAppLoad();
    // CFRunLoopRun();
}
