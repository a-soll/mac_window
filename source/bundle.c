#include "include/bundle.h"

// get app CFURL up to .app for launching
void getAppPath(char path[MAXPATHLEN]) {
    char to[MAXPATHLEN];
    char *tmp;

    // we need to slice off everything after .app in path
    tmp = strstr(path, ".app");
    if (tmp) {
        int ind = (tmp - path) + 4;
        strncpy(to, path, ind);
        to[ind] = '\0';
        strcpy(path, to);
    }
}

CFURLRef loadBundle(char *app_path) {
    getAppPath(app_path);
    CFURLRef url;
    CFStringRef path = CFStringCreateWithCString(kCFAllocatorDefault, (const char *)app_path, kCFStringEncodingUTF8);
    url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, path, kCFURLPOSIXPathStyle, true);
    CFRelease(path);
    return url;
}
