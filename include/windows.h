#ifndef WINDOWS_H
#define WINDOWS_H

#include "base.h"
#include "display.h"
#include <ApplicationServices/ApplicationServices.h>

/**Window attributes
 * CGPoint position = current position
 * CGSize size = current size
 * AXUIElementRef = UI element for modifications
 * uint32_t wid = window ID
 * int displayIndex = index of the Display in displayList that it's currently on
 */
typedef struct Window {
    CGPoint position;
    CGSize size;
    AXUIElementRef uiElem;
    uint32_t wid;
    int displayIndex;
    CGPoint topright;
    CGPoint topleft;
    CGPoint bottomright;
    CGPoint bottomleft;
} Window;

typedef enum {
    wTopLeft,
    wTopRight,
    wBottomLeft,
    wBottomRight,
} wcorner_t;

/*!
    @function getWindowList
    populates an array of Window structs for all currently visible windows
    @param Window pointer to Window struct. will initialize as array of Windows
*/
int getWindowList(Window **w);

// gets windows by their name
int getWindowByName(Window *w, int count, const char *appName);

// move the given app window
void windowMove(Window *w, double x, double y);

// resize given app window
void windowResize(Window *w, double width, double height);

void windowMoveByCorner(Window *w, wcorner_t corner, double x, double y);

// properly free Window list
void releaseWindowList(Window *w, int count);

// free just one window
void releaseWindow(Window *w);

// populates Window struct
void initWindow(Window *w, CFArrayRef UIElems, int count);

void windowGetDisplay(Window *w);

double windowMoveLeftEdgeFlush(Window *w);

#endif /* WINDOWS_H */
