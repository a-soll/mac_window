#ifndef BASE_H
#define BASE_H

// non-struct includes
#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include <ApplicationServices/ApplicationServices.h>
#include "CGSSpaces.h"
#include <stdio.h>

int g_connection;

typedef enum {
    cTopLeft,
    cTopRight,
    cBottomLeft,
    cBottomRight,
} corner_t;

#endif /* BASE_H */
