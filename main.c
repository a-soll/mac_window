#include "includes.h"
#include <ApplicationServices/ApplicationServices.h>
#include <CoreGraphics/CoreGraphics.h>
#include <NSObjects/NSBase.h>

int main() {
    Window *w = NULL;
    CFTypeRef pos;
    CFTypeRef size;

    // get window of app
    int numWindows = getWindowList(&w);

    for (int i = 0; i < numWindows; i++) {
        printf("%d\n", w[i].pid);
    }
    free(w);
    //     // create accessibility object by app's pid
    //     AXUIElementRef elem = AXUIElementCreateApplication(window.pid);

    //     // populate array of all UI element values for the windows
    //     AXUIElementCopyAttributeValue(elem, kAXWindowsAttribute, (CFTypeRef *)&window);

    //     // create UIElement for item in first spot of array
    //     AXUIElementRef windowRef = (AXUIElementRef)CFArrayGetValueAtIndex(window, 0);

    //     // copies the position attribute to pos
    //     AXUIElementCopyAttributeValue(windowRef, kAXPositionAttribute, (CFTypeRef *)&pos);
    //     AXUIElementCopyAttributeValue(windowRef, kAXSizeAttribute, (CFTypeRef *)&size);

    //     CGPoint curPosition;
    //     AXValueGetValue(pos, kAXValueCGPointType, &curPosition);

    //     CGSize curSize;
    //     AXValueGetValue(size, kAXValueCGSizeType, &curSize);

    //     printf("POSITION\n");
    //     printf("x   %f\ny   %f\n\n", w[0]->position.x, w[0]->position.y);

    //     printf("SIZE\n");
    //     printf("width    %f\nheight    %f\n", w[0]->size.width, w[0]->size.height);

    //     // move window to new position
    //     CGPoint newPosition = {0, 0};
    //     pos = (CFTypeRef)(AXValueCreate(kAXValueCGPointType, (const void *)&newPosition));
    //     AXUIElementSetAttributeValue(windowRef, kAXPositionAttribute, pos);

    //     sleep(1);

    //     // move window back to old position
    //     pos = (CFTypeRef)(AXValueCreate(kAXValueCGPointType, (const void *)&curPosition));
    //     AXUIElementSetAttributeValue(windowRef, kAXPositionAttribute, pos);
    //     return 0;
    // }
}
