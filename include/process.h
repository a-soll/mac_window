#ifndef PROCESS_H
#define PROCESS_H

#include "base.h"
#include "hash.h"

Bucket *proc_array[125];
Bucket *proc_bucket;

typedef struct Proccess {
    pid_t pid;
    ProcessSerialNumber psn;
    char name[NAME_LEN];
} Process;

int getProcessList(Process **p);

Process *getProcessByPid(pid_t pid);

void procBucketAdd(Process *process);

#endif /* PROCESS_H */
