#include "includes.h"
#include <ApplicationServices/ApplicationServices.h>
#include <CoreGraphics/CoreGraphics.h>
#include <NSObjects/NSBase.h>

int main() {
    Window *w = NULL;
    CFArrayRef windows;
    CFTypeRef pos;
    CFTypeRef size;

    // get window of app
    int count = getWindowList(&w);
    int ind = getWindowByName(w, count, "Safari");
    Window app = w[ind];
    printf("%s\n", app.name);
    // AXUIElementCopyAttributeNames(CFArrayGetValueAtIndex(app.arr, 0), &windows);
    CFShow(app.uiElements);
    printf("%d\n", app.pid);
    printf("%f\n", app.size.width);
    // moveWindow(&app, 10, 10);

    // // create accessibility object by app's pid
    // AXUIElementRef elem = AXUIElementCreateApplication(app.pid);

    // // populate array of all UI element values for the windows
    // AXUIElementCopyAttributeValue(elem, kAXWindowsAttribute, (CFTypeRef *)&windows);

    releaseWindow(w, count);
    exit(0);
    // create accessibility object by app's pid
    AXUIElementRef elem = AXUIElementCreateApplication(w[0].pid);

    // populate array of all UI element values for the windows
    AXUIElementCopyAttributeValue(elem, kAXWindowsAttribute, (CFTypeRef *)&windows);

    // create UIElement for item in first spot of array
    AXUIElementRef windowRef = (AXUIElementRef)CFArrayGetValueAtIndex(windows, 0);

    // copies the position attribute to pos
    AXUIElementCopyAttributeValue(windowRef, kAXPositionAttribute, (CFTypeRef *)&pos);
    AXUIElementCopyAttributeValue(windowRef, kAXSizeAttribute, (CFTypeRef *)&size);

    CGPoint curPosition;
    AXValueGetValue(pos, kAXValueCGPointType, &curPosition);

    CGSize curSize;
    AXValueGetValue(size, kAXValueCGSizeType, &curSize);

    printf("POSITION\n");
    printf("x   %f\ny   %f\n\n", curPosition.x, curPosition.y);

    printf("SIZE\n");
    printf("width    %f\nheight    %f\n", curSize.width, curSize.height);

    // move window to new position
    CGPoint newPosition = {0, 0};
    pos = (CFTypeRef)(AXValueCreate(kAXValueCGPointType, (const void *)&newPosition));
    AXUIElementSetAttributeValue(windowRef, kAXPositionAttribute, pos);

    sleep(1);

    // move window back to old position
    pos = (CFTypeRef)(AXValueCreate(kAXValueCGPointType, (const void *)&curPosition));
    AXUIElementSetAttributeValue(windowRef, kAXPositionAttribute, pos);
    return 0;
}
