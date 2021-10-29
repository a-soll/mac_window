#ifndef PROCESS_H
#define PROCESS_H

#include "base.h"
#include "bucket.h"

typedef struct Proccess {
    pid_t pid;
    ProcessSerialNumber psn;
    char name[APP_NAME_MAX];
    bool xpc;
} Process;

void get_process_list();
Process *get_process(ProcessSerialNumber psn);
void remove_process(ProcessSerialNumber psn);
Process *init_process(ProcessSerialNumber psn);

#endif /* PROCESS_H */
