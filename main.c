#include "include/CGSSpaces.h"
#include "include/base.h"
#include "include/includes.h"
#include "include/notif.h"

int g_connection;

int main() {
    // init global variables
    g_connection = SLSMainConnectionID();
    Display *displayList = NULL;
    Application *apps = NULL;
    Window *windows = NULL;
    Process *process = NULL;

    displayCount = getDisplayList(&displayList);
    void *g_workspace_context;
    workspace_event_handler_init(&g_workspace_context);
    workspace_event_handler_begin(&g_workspace_context);

    // int count = getWindowList(&windows);
    // int count = getOpenApplications(&apps);
    int count = getProcessList(&process);
    printf("COUNT %d\n", count);
    // initWindow(&windows);
    // bridgeNSAppLoad();
    // CFRunLoopRun();
    exit(0);

//     for (int i = 0; i < count; i++) {
//         Application *app = &apps[i];
//         if (strcmp(app->name, "Code") == 0) {
//             for (int j = 0; j < app->windowCount; j++) {
//                 Window *w = &app->windows[j];
//                 Display *d = &displayList[w->displayIndex];
//                 CGPoint oldPos = w->position;
//                 CGSize oldSize = w->size;

//                 printf("APP: %s\n", app->name);
//                 printf("TOPLEFT: %f\t%f\n", w->topleft.x, w->topleft.y);
//                 // printf("%f\n", w->topright.x);
//                 // printf("%f\n", w->topright.y);
//                 // windowResize(w, w->size.width, d->height);
//                 // windowResize(w, oldSize.width, oldSize.height);
//                 exit(0);
//                 // windowGetDisplay(w);
//                 // printf("x: %f\n", w->position.x);
//                 // printf("y: %f\n\n", w->position.y);
//                 // break;
//                 // printf("dx: %f\n", displayList[w->displayIndex].width);
//                 // printf("dy: %f\n\n", displayList[w->displayIndex].height);
//                 // windowMove(w, 0, 1440);
//                 // printf("width: %f   height: %f\n", w->size.width, w->size.height);
//                 // resizeWindow(w, display.width, display.height);
//                 sleep(2);
//                 windowMove(w, oldPos.x, oldPos.y);
//                 // break;
//                 // break;
//                 windowResize(w, oldSize.width, oldSize.height);
//                 break;
//             }
//         }
//     }

//     for (int i = 0; i < displayCount; i++) {
//         Display d = displayList[i];
//         printf("%f\n", d.width);
//     }
//     releaseApplicationList(apps, count);
//     releaseDisplayList(displayList, displayCount);
}
