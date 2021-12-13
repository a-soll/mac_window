#ifndef BASE_H
#define BASE_H

#define MAX_DISPLAYS 5
#define APP_NAME_MAX 256
#define arrayCount(a) (sizeof((a)) / sizeof(*(a)))
#define kCPSAllWindows 0x100
#define kCPSUserGenerated 0x200
#define kCPSNoWindows 0x400

// non-struct includes
#include "CGSSpaces.h"
#include <ApplicationServices/ApplicationServices.h>
#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include <libproc.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

typedef enum {
    cTopLeft,
    cTopRight,
    cBottomLeft,
    cBottomRight,
} corner_t;

CFStringRef cfstring(char *cstring);
CFStringRef c_cfstring(const char *cstring);
CFArrayRef cfarray_from_numbers(void *values, size_t size, int count, CFNumberType type);
int get_cfnumber_from_array(CFArrayRef arr, int ind);
char *get_cstring(CFStringRef from);
int fmt_string(char *to, const char *s, ...);

#endif /* BASE_H */
