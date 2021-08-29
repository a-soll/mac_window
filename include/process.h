#ifndef PROCESS_H
#define PROCESS_H

#include "base.h"
#include "bucket.h"

Bucket *proc_bucket;

typedef struct Proccess {
    pid_t pid;
    ProcessSerialNumber psn;
    char name[APP_NAME_MAX];
} Process;

void getProcessList();

Process *getProcess(ProcessSerialNumber psn);

void removeProcess(ProcessSerialNumber psn);

#endif /* PROCESS_H */
