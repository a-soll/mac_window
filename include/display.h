#ifndef DISPLAY_H
#define DISPLAY_H

#include "base.h"

#define MAX_DISPLAYS 5

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

int displayCount;
Display *displayList;

int getDisplayList(Display **d);

void currentDisplay(Display **d);

void releaseDisplayList(Display *d, int count);

#endif /* DISPLAY_H */
