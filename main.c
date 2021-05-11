#include "includes.h"
#include <ApplicationServices/ApplicationServices.h>
#include <CoreGraphics/CoreGraphics.h>

int main() {
    Window *w = NULL;

    // populate array of Windows for all open apps
    int count = getWindowList(&w);

    // create singular Window for given app name
    int ind = getWindowByName(w, count, "Code");
    Window app = w[ind];

    CGPoint newPosition = {0, 0};
    CGPoint curPosition = app.position;

    // move window to new position, sleep, move back
    moveWindow(&app, newPosition);
    sleep(1);
    moveWindow(&app, curPosition);

    // free windows
    releaseWindow(w, count);
}
