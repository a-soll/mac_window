#include "include/bundle.h"

// get app CFURL up to .app for launching
void get_app_path(char path[MAXPATHLEN]) {
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

CFURLRef load_bundle(char *app_path) {
    get_app_path(app_path);
    CFURLRef url;
    CFStringRef path = CFStringCreateWithCString(kCFAllocatorDefault, (const char *)app_path, kCFStringEncodingUTF8);
    url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, path, kCFURLPOSIXPathStyle, true);
    CFRelease(path);
    return url;
}
