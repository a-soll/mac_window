#ifndef BASE_H
#define BASE_H

#define MAX_DISPLAYS 5
#define APP_NAME_MAX 256
#define arrayCount(a) (sizeof((a)) / sizeof(*(a)))

// non-struct includes
#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include <ApplicationServices/ApplicationServices.h>
#include "CGSSpaces.h"
#include <stdio.h>
#include <string.h>
#include <libproc.h>

typedef enum {
    cTopLeft,
    cTopRight,
    cBottomLeft,
    cBottomRight,
} corner_t;

// CFStringRef from char
CFStringRef CFSTRING(char *cstring);
// CFStringRef from const char
CFStringRef CCFSTRING(const char *cstring);
CFArrayRef CFArrayFromNumbers(void *values, size_t size, int count, CFNumberType type);
int getNumberFromArray(CFArrayRef arr, int ind);

#endif /* BASE_H */
