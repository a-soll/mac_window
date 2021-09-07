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

uint64_t *spaceListForDisplay(int did);

#endif /* DISPLAY_H */
