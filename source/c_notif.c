#include "include/base.h"
#include "include/notif.h"
#include "include/includes.h"
#include <time.h>

void spaceDidChange(const char *notif) {
    time_t now;
    time(&now);
    printf("%s %s\n", ctime(&now), notif);
    getWindowList();
}

void bridgeNSAppLoad() {
    NSAppLoad();
}
