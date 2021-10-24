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

void initSpaceList();
// return all window IDs for given space ID
CFArrayRef spaceWindows(uint64_t sid);
uint64_t getActiveSpace();

#endif /* SPACE_H */
