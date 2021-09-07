#include "include/bundle.h"

void getAppBundle() {
    CFURLRef url;
    CFURLCreateWithString(kCFAllocatorDefault, CFSTR("/Applications"), url);
    CFArrayRef bundles = CFBundleCreateBundlesFromDirectory(kCFAllocatorDefault, url, CFSTR("bundle"));
    url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, CFSTR("/Applications"), kCFURLPOSIXPathStyle, true);
    CFArrayRef idk = CFBundleCreateBundlesFromDirectory(kCFAllocatorDefault, url, NULL);
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

CFURLRef loadBundle(CFStringRef URLString) {
    CFURLRef url;
    url = CFURLCreateWithString(kCFAllocatorDefault, CFSTR("file:///Applications/gfxCardStatus.app/"), NULL);

    return url;
}
