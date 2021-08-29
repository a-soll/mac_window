#ifndef BASE_H
#define BASE_H

#define MAX_DISPLAYS 5
#define NAME_LEN 30

// non-struct includes
#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include <ApplicationServices/ApplicationServices.h>
#include "CGSSpaces.h"
#include <stdio.h>

typedef enum {
    cTopLeft,
    cTopRight,
    cBottomLeft,
    cBottomRight,
} corner_t;

#endif /* BASE_H */
