#include "include/process.h"

#pragma clang diagnostic ignored "-Wdeprecated-declarations"
Process *initProcess(ProcessSerialNumber psn) {
    Process *process;
    pid_t pid;
    ProcessInfoRec process_info = {.processInfoLength = sizeof(ProcessInfoRec)};
    GetProcessInformation(&psn, &process_info);
    GetProcessPID(&psn, &pid);
    CFStringRef name_ref;
    CopyProcessName(&psn, &name_ref);
    process = malloc(sizeof(Process));
    CFStringGetCString(name_ref, process->name, NAME_LEN, kUnicodeUTF8Format);
    process->pid = pid;
    process->psn = psn;
    return process;
}

Process *getProcessByPid(pid_t pid) {
    Process *process = (Process *)bucket_search(pid, proc_array, proc_bucket->capacity);
}

void procBucketAdd(Process *process) {
    proc_bucket = malloc(sizeof(Bucket));
    proc_bucket->capacity = 125;
    bucket_insert(proc_bucket, proc_array, process->pid, process);
}

int getProcessList(Process **p) {
    int count = 0;
    Process *process;

    ProcessSerialNumber psn = {kNoProcess, kNoProcess};
    while (GetNextProcess(&psn) == noErr) {
        process = initProcess(psn);
        printf("%d\n", process->pid);
        procBucketAdd(process);
        count++;
    }
    searchProcBucket(25177);
    return count;
}
