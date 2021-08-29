#include "include/display.h"

extern int g_connection;

void getDisplayDimensions(Display *d, CGDirectDisplayID did) {
    d->did = did;
    d->width = (double)CGDisplayPixelsWide(d->did);
    d->height = (double)CGDisplayPixelsHigh(d->did);

    // get corner coords
    d->topleft.x = d->origin.x;
    d->topleft.y = d->origin.y;

    d->topright.x = d->width + d->origin.x;
    d->topright.y = d->origin.y;

    d->bottomleft.x = d->origin.x;
    d->bottomright.y = d->origin.y + d->height;

    d->bottomright.x = d->width + d->origin.x;
    d->bottomright.y = d->height + d->origin.y;
}

int getDisplayList(Display **d) {
    Display *display;
    CFArrayRef display_list = SLSCopyManagedDisplays(g_connection);
    uint32_t count = CFArrayGetCount(display_list);
    CGDirectDisplayID ids[count];
    CGGetActiveDisplayList(count, ids, &count);

    // reallocate on consecutive calls
    if (*d == NULL) {
        display = malloc(count * sizeof(Display));
    } else {
        display = realloc(*d, count * sizeof(Display));
    }

    for (int i = 0; i < count; i++) {
        Display d;
        CGDirectDisplayID curId = ids[i];
        CGRect origin = CGDisplayBounds(curId);
        d.origin = origin.origin;
        if (curId == CGMainDisplayID()) {
            d.isMain = true;
        } else {
            d.isMain = false;
        }
        d.uuid = CFArrayGetValueAtIndex(display_list, i);
        getDisplayDimensions(&d, curId);
        CFRetain(d.uuid);
        display[i] = d;
    }
    CFRelease(display_list);
    *d = display;
    return (int)count;
}

void releaseDisplayList(Display *d, int count) {
    for (int i = 0; i < count; i++) {
        if (d[i].uuid != NULL) {
            CFRelease(d[i].uuid);
        }
    }
    free(d);
}
