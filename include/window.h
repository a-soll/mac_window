#ifndef WINDOWS_H
#define WINDOWS_H

#include "base.h"
#include "application.h"
#include "display.h"
#include "bucket.h"
#include "process.h"
#include "c_callback.h"

/**Window attributes
 * CGPoint position = current position
 * CGSize size = current size
 * AXUIElementRef = UI element for modifications
 * uint32_t wid = window ID
 * int displayIndex = index of the Display in displayList that it's currently on
 */
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
} Window;

// actual CSTR names of notifs
static const char* const notifs[] = {
    "AXResized",
    "AXWindowDeminiaturized",
    "AXWindowMiniaturized"
};

/*!
    @function getWindowList
    populates an array of Window structs for all currently visible windows
    @param Window pointer to Window struct. will initialize as array of Windows
*/
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

#endif /* WINDOWS_H */
