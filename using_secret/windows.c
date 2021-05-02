#include "includes.h"
#include <ApplicationServices/ApplicationServices.h>
// #include <CGSInternal/CGSSpace.h>
#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CGDisplayConfiguration.h>
#include <CoreGraphics/CoreGraphics.h>
#include <NSObjects/NSBase.h>
// #include "util/CGSPrivate.h"
#include "util/CGSSpaces.h"
// #include <CGSInternal/CGSWorkspace.h>
// #include <CGSInternal/CGSConnection.h>
// #include <Carbon/Carbon.h>

//Declarations to avoid compiler warnings (because of private APIs):
// typedef void * CGSConnection;
// extern CGSConnectionID _CGSDefaultConnection(void);
// extern CGError CGSGetWorkspaceWindowCount(CGSConnectionID cid, int workspaceNumber, int *outCount);
// extern OSStatus CGSGetWorkspaceWindowCount(const CGSConnectionID cid, int workspaceNumber, int *outCount);
extern AXError _AXUIElementGetWindow(AXUIElementRef ref, uint32_t *wid);
// extern CGSConnectionID _CGSDefaultConnection(void);
extern CGError SLSGetWindowOwner(int cid, uint32_t wid, int *wcid);
// extern CGError SLSGetConnectionPSN(int cid, ProcessSerialNumber *psn);
int g_connection;

int main() {
    Window *windows = NULL;
    CFTypeRef pos;
    CFTypeRef size;
    CGDirectDisplayID displays[32];
    uint32_t count;
    CGWindowID list;
    uint32_t wid;
    int dock;
    int elemcon;
    // cgswindow w;
    int n;

    int numWindows = getWindowList(&windows);

    for (int i = 0; i < numWindows; i++) {
        if (strcmp(windows[i].name, "Dock") == 0) {
            CFArrayRef winref;
            AXUIElementRef elem = AXUIElementCreateApplication(windows[i].pid);
            CFShow(elem);
            AXUIElementCopyAttributeValue(elem, kAXWindowsAttribute, (CFTypeRef *)&winref);
            _AXUIElementGetWindow(elem, &wid);
            printf("%d\n", wid);
            SLSGetWindowOwner(g_connection, wid, &dock);
            // CGSGetWindowOwner(_CGSDefaultConnection(), wid, &dock);
            // ProcessSerialNumber psn;
            // SLSGetConnectionPSN(dock, &psn);
            // CGSGetConnectionIDForPSN(n)
            printf("%d\n", n);
        }
    }
    // printf("%s\n", windows[i].name);
    // AXUIElementRef windowRef = (AXUIElementRef)CFArrayGetValueAtIndex(winref, 0);
    // AXUIElementCopyAttributeValue(windowRef, kAXPositionAttribute, (CFTypeRef *)&pos);
    // AXUIElementCopyAttributeValue(windowRef, kAXSizeAttribute, (CFTypeRef *)&size);

    // CGPoint curPosition;
    // AXValueGetValue(pos, kAXValueCGPointType, &curPosition);

    // CGSize curSize;
    // AXValueGetValue(size, kAXValueCGSizeType, &curSize);

    // printf("POSITION\n");
    // printf("x   %f\ny   %f\n\n", curPosition.x, curPosition.y);

    // printf("SIZE\n");
    // printf("width    %f\nheight    %f\n", curSize.width, curSize.height);
    // }
    // free(windows);
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
