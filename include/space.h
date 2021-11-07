#ifndef SPACE_H
#define SPACE_H

#include "application.h"
#include "base.h"
#include "bucket.h"
#include "display.h"

typedef struct Space {
    char label[256];
    uint64_t sid;
} Space;

typedef enum {
    /// User-created desktop spaces.
    CGSSpaceTypeUser = 0,
    /// Fullscreen spaces.
    CGSSpaceTypeFullscreen = 1,
    /// System spaces e.g. Dashboard.
    CGSSpaceTypeSystem = 2,
} CGSSpaceType;

void init_space_list();
// return all window IDs for given space ID
CFArrayRef space_windows(uint64_t sid);
uint64_t get_active_space();
Space *create_space(CFStringRef uuid);
Space *get_space(uint64_t sid);

#endif /* SPACE_H */
