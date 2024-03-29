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

    // FILE *f = fopen("test", "w");
    // size_t size = sizeof(w);
    // fwrite(w, size, 1, f);
    // launch_application(w->application);

    // Window *w = (Window *)table_iterate(window_table, true);
    // do {
    //     uint64_t sid = current_space_for_window(w);
    //     printf("WID: %d\n", w->wid);
    //     printf("Name: %s\n", w->application->name);
    //     CFShow(w->uiElem);
    //     printf("Height: %f\n", w->size.height)
    //     printf("Width: %f\n", w->size.width);
    //     printf("SID: %llu\n", sid);
    //     printf("Mini? %d\n", w->isMinimized);
    //     printf("path: %s\n", w->application->path);
    //     printf("\n");
    // } while ((w = (Window *)table_iterate(window_table, false)));

    char surl[PROC_PIDPATHINFO_MAXSIZE] = "/System/Applications/Safari.app";
    CFURLRef url = load_bundle(surl);
    CFShow(url);
    CFRelease(url);
    // launch_application(url);
    // snapshot();
    INI *ini;
    int ini_len = load_config(&ini);

    for (int i = 0; i < ini_len; i++) {
        printf("wid: %d\n", ini[i].wid);
        printf("space: %d\n", ini[i].space);
        printf("height: %f\n", ini[i].size.height);
        printf("width: %f\n", ini[i].size.width);
        printf("x: %f\n", ini[i].position.x);
        printf("y: %f\n", ini[i].position.y);
    }

    // for (int i = 0; i < ini_len; i++) {
    //     printf("%s\n", ini[i].path);
    //     launch_application(ini[i].path);
    // }
    bridgeNSAppLoad();
    // CFRunLoopRun();
    table_free(proc_table);
    table_free(app_table);
    table_free(window_table);
    table_free(display_table);
    free(ini);
}
