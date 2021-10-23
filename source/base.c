#include "include/base.h"

CFStringRef CCFSTRING(const char *cstring) {
    CFStringRef ref;
    ref = CFStringCreateWithCString(kCFAllocatorDefault, cstring, kCFStringEncodingUTF8);
    return ref;
}
