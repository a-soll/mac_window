#ifndef WINDOWS_H
#define WINDOWS_H

#include "base.h"
#include "application.h"
#include "display.h"
#include "bucket.h"
#include "process.h"

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

void get_window_list();
Window *get_window(uint32_t wid);
// move the given app window
CGPoint window_move(Window *w, double x, double y);
// resize given app window
CGSize window_resize(Window *w, double width, double height);
void window_move_by_corner(Window *w, corner_t corner, double x, double y);
void release_window(void *window);
void init_window(CFArrayRef window_list, Application *application);
void window_get_display(Window *w);
double window_move_left_edge_flush(Window *w);
void window_get_size(Window *window);
void window_get_position(Window *window);
uint64_t current_space_for_window(Window *window);
void move_window_to_space(uint32_t wid, uint64_t sid);
AXError set_focused_window(Application *application, uint32_t wid);

#endif /* WINDOWS_H */
