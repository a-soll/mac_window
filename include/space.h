#ifndef SPACE_H
#define SPACE_H

#include "base.h"
#include "display.h"
#include "bucket.h"
#include "application.h"

typedef struct Space {
    char label[256];
    uint64_t sid;
} Space;

void init_space_list();
// return all window IDs for given space ID
CFArrayRef space_windows(uint64_t sid);
uint64_t get_active_space();

#endif /* SPACE_H */
