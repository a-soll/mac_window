#ifndef CONFIG_H
#define CONFIG_H

#include "base.h"
#include "includes.h"

typedef struct INI {
    char path[PATH_MAX];
    int wid;
    int space;
    CGPoint position;
    CGSize size;
} INI;

void write_file(char *buffer);
void snapshot();
int load_config(INI **ini);

#endif /* CONFIG_H */
