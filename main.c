#include "include/CGSSpaces.h"
#include "include/base.h"
#include "include/includes.h"
#include "include/notif.h"

int g_connection;
Table *proc_table;

int main() {
    g_connection = SLSMainConnectionID();
    Display *displayList = NULL;

    displayCount = getDisplayList(&displayList);
    void *g_workspace_context;
    workspace_event_handler_init(&g_workspace_context);
    workspace_event_handler_begin(&g_workspace_context);

    getProcessList();
    getWindowList();

    table_free(proc_table);
    // bridgeNSAppLoad();
    // CFRunLoopRun();
}
