#include "include/base.h"

// CFStringRef from const char
CFStringRef c_cfstring(const char *cstring) {
    CFStringRef ref;
    ref = CFStringCreateWithCString(kCFAllocatorDefault, cstring, kCFStringEncodingUTF8);
    return ref;
}

// CFStringRef from char
CFStringRef cfstring(char *cstring) {
    CFStringRef ref;
    ref = CFStringCreateWithCString(kCFAllocatorDefault, cstring, kCFStringEncodingUTF8);
    return ref;
}

char *get_cstring(CFStringRef from) {
    CFIndex length = CFStringGetLength(from);
    CFIndex max_size = CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8) + 1;
    char *buffer = malloc(max_size);

    if (CFStringGetCString(from, buffer, max_size, kCFStringEncodingUTF8)) {
        return buffer;
    }
    free(buffer);
    return NULL;
}

CFArrayRef cfarray_from_numbers(void *values, size_t size, int count, CFNumberType type) {
    CFNumberRef tmp[count];

    for (int i = 0; i < count; i++) {
        tmp[i] = CFNumberCreate(NULL, type, ((char *)values) + (size * i));
    }
    CFArrayRef result = CFArrayCreate(kCFAllocatorDefault, (const void **)tmp, count, &kCFTypeArrayCallBacks);
    return result;
}

int get_cfnumber_from_array(CFArrayRef arr, int ind) {
    int num;
    CFNumberRef tmp = CFArrayGetValueAtIndex(arr, ind);
    CFNumberGetValue(tmp, kCFNumberSInt32Type, &num);
    CFRelease(tmp);
    return num;
}

/**
 * call with string to store result, string to format,
 * and variables to append to format string.
 *
 * ex: fmt_string(to, "hello %s", name);
 */
int fmt_string(char *to, const char *s, ...) {
    va_list ap;
    int ret;

    va_start(ap, s);
    ret = vsprintf(to, s, ap);
    va_end(ap);

    return ret;
}
