#include "include/process.h"

extern Table *proc_table;

#pragma clang diagnostic ignored "-Wdeprecated-declarations"
Process *initProcess(ProcessSerialNumber psn) {
    Process *process;
    pid_t pid;
    ProcessInfoRec process_info = {.processInfoLength = sizeof(ProcessInfoRec)};
    GetProcessInformation(&psn, &process_info);
    GetProcessPID(&psn, &pid);
    CFStringRef name_ref;
    if (name_ref != NULL) {
        CopyProcessName(&psn, &name_ref);
        process = malloc(sizeof(Process));
        process->xpc = process_info.processType == 'XPC!';
        CFStringGetCString(name_ref, process->name, APP_NAME_MAX, kUnicodeUTF8Format);
        process->pid = pid;
        process->psn = psn;
        CFRelease(name_ref);
    }
    return process;
}

Process *getProcess(ProcessSerialNumber psn) {
    Process *process = (Process *)table_search(proc_table, psn.lowLongOfPSN);
    return process;
}

void removeProcess(ProcessSerialNumber psn) {
    table_delete_item(proc_table, psn.lowLongOfPSN);
}

void getProcessList() {
    Process *process;
    proc_table->release = NULL;
    ProcessSerialNumber psn = {kNoProcess, kNoProcess};

    while (GetNextProcess(&psn) == noErr) {
        process = initProcess(psn);
        if (process != NULL) {
            if (!process->xpc && strcmp(process->name, "Finder") != 0 && strcmp(process->name, "Dock") != 0) {
                table_insert(proc_table, process->psn.lowLongOfPSN, (void *)process);
            }
            else {
                free(process);
            }
        }
    }
}
