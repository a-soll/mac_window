#ifndef DISPLAY_H
#define DISPLAY_H

#include "base.h"
#include "bucket.h"
#include "space.h"

typedef struct Display {
    bool isMain;
    int did;
    CFStringRef uuid;
    double width;
    double height;
    CGPoint origin;
    CGPoint topleft;
    CGPoint topright;
    CGPoint bottomleft;
    CGPoint bottomright;
} Display;

void get_display_list();
void release_display(void *display);
int space_list_for_display(int did, uint64_t **sid_list);
CFStringRef get_display_for_window_id(uint64_t wid);
Display *get_display(int did);
Display *get_main_display();

#endif /* DISPLAY_H */
