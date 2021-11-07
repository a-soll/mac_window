#include "include/base.h"
#include "include/includes.h"
#include "include/notif.h"
#include <time.h>

void space_did_change(const char *notif) {
    time_t now;
    time(&now);
    printf("%s %s\n", ctime(&now), notif);
    get_window_list();
}

void bridgeNSAppLoad() {
    NSAppLoad();
}
