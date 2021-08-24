#ifndef NOTIF_H
#define NOTIF_H

#include <stdbool.h>

void spaceDidChange(const char *notif);
bool NSAppLoad(void);
void workspace_event_handler_init(void **context);
void workspace_event_handler_begin(void **context);
void bridgeNSAppLoad();

#endif /* NOTIF_H */
