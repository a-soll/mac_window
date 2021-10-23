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

// don't need lul
void getAppBundle() {
    CFURLRef url;
    CFURLCreateWithString(kCFAllocatorDefault, CFSTR("/Applications"), url);
    CFArrayRef bundles = CFBundleCreateBundlesFromDirectory(kCFAllocatorDefault, url, CFSTR("bundle"));
    url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, CFSTR("/Applications"), kCFURLPOSIXPathStyle, true);
    CFArrayRef idk = CFBundleCreateBundlesFromDirectory(kCFAllocatorDefault, url, NULL);
    CFShow(bundles);
    int count = CFArrayGetCount(idk);

    for (int i = 0; i < count; i++) {
        CFBundleRef bref = (CFBundleRef)CFArrayGetValueAtIndex(idk, i);
        CFDictionaryRef dref = CFBundleGetInfoDictionary(bref);
        CFShow(dref);
        CFURLRef aurl = CFBundleCopyBundleURL(bref);
        CFShow(aurl);
        LSOpenCFURLRef(aurl, NULL);
        break;
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
