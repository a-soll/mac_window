#include "include/CGSSpaces.h"
#include "include/base.h"
#include "include/includes.h"
#include <ApplicationServices/ApplicationServices.h>

// probably will always be prototype/test code
int main() {
    // init global variables
    g_connection = SLSMainConnectionID();
    displayList = NULL;
    displayCount = getDisplayList(&displayList);

    Application *apps = NULL;
    int count = getOpenApplications(&apps);

    for (int i = 0; i < count; i++) {
        Application *app = &apps[i];
        if (strcmp(app->name, "Messages") == 0) {
            for (int j = 0; j < app->windowCount; j++) {
                Window *w = &app->windows[j];
                Display *d = &displayList[w->displayIndex];
                CGPoint oldPos = w->position;
                CGSize oldSize = w->size;

                // printf("%f\n", w->topright.x);
                // printf("%f\n", w->topright.y);
                // printf("%f\n", d->topright.y - w->topright.y);
                windowMoveByCorner(w, wBottomRight, 3026, 767);
                exit(0);
                windowMove(w, d->width - w->size.width, d->origin.y);
                // windowGetDisplay(w);
                // printf("x: %f\n", w->position.x);
                // printf("y: %f\n\n", w->position.y);
                // break;
                // printf("dx: %f\n", displayList[w->displayIndex].width);
                // printf("dy: %f\n\n", displayList[w->displayIndex].height);
                // windowMove(w, 0, 1440);
                // printf("width: %f   height: %f\n", w->size.width, w->size.height);
                // resizeWindow(w, display.width, display.height);
                sleep(2);
                windowMove(w, oldPos.x, oldPos.y);
                // break;
                // break;
                windowResize(w, oldSize.width, oldSize.height);
                break;
            }
        }
    }

    for (int i = 0; i < displayCount; i++) {
        Display d = displayList[i];
        // printf("%f\n", d.width);
    }
    releaseApplicationList(apps, count);
    releaseDisplayList(displayList, displayCount);
}
