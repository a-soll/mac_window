#ifndef SPACE_H
#define SPACE_H

#include "base.h"
#include "display.h"
#include "bucket.h"

typedef struct Space {
    char label[256];
    uint64_t sid;
} Space;

void initSpaceList();

#endif /* SPACE_H */
