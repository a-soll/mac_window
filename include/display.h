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

void getDisplayList();
void currentDisplay(Display **d);
void releaseDisplay(void *display);
int spaceListForDisplay(int did, uint64_t **sid_list);
CFStringRef getDisplayForWindowId(uint64_t wid);

#endif /* DISPLAY_H */
