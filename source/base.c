#include "include/base.h"

CFStringRef CCFSTRING(const char *cstring) {
    CFStringRef ref;
    ref = CFStringCreateWithCString(kCFAllocatorDefault, cstring, kCFStringEncodingUTF8);
    return ref;
}

CFArrayRef CFArrayFromNumbers(void *values, size_t size, int count, CFNumberType type) {
    CFNumberRef tmp[count];

    for (int i = 0; i < count; i++) {
        tmp[i] = CFNumberCreate(NULL, type, ((char *)values) + (size * i));
    }
    CFArrayRef result = CFArrayCreate(kCFAllocatorDefault, (const void **)tmp, count, &kCFTypeArrayCallBacks);
    return result;
}

int getNumberFromArray(CFArrayRef arr, int ind) {
    int num;
    CFNumberRef tmp = CFArrayGetValueAtIndex(arr, ind);
    CFNumberGetValue(tmp, kCFNumberSInt32Type, &num);
    CFRelease(tmp);
    return num;
}
