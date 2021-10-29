#ifndef WINDOWS_H
#define WINDOWS_H

#include "base.h"
#include "application.h"
#include "display.h"
#include "bucket.h"
#include "process.h"
#include "c_callback.h"

typedef struct Window {
    Application *application;
    CGPoint position;
    CGSize size;
    AXUIElementRef uiElem;
    uint32_t wid;
    int displayIndex;
    CGPoint topright;
    CGPoint topleft;
    CGPoint bottomright;
    CGPoint bottomleft;
    bool isMinimized;
    uint64_t space;
} Window;

// actual CSTR names of notifs
static const char* const notifs[] = {
    "AXResized",
    "AXWindowDeminiaturized",
    "AXWindowMiniaturized",
    "AXWindowResized"
};

void getWindowList();
Window *getWindow(uint32_t wid);
// move the given app window
CGPoint windowMove(Window *w, double x, double y);
// resize given app window
CGSize windowResize(Window *w, double width, double height);
void windowMoveByCorner(Window *w, corner_t corner, double x, double y);
void releaseWindow(void *window);
void initWindow(CFArrayRef window_list, Application *application);
void windowGetDisplay(Window *w);
double windowMoveLeftEdgeFlush(Window *w);
void windowGetSize(Window *window);
void windowGetPosition(Window *window);
uint64_t currentSpaceForWindow(Window *window);
void moveWindowToSpace(uint32_t wid, uint64_t sid);
AXError setFocusedWindow(Application *application, uint32_t wid);

#endif /* WINDOWS_H */
