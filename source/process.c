#include "include/process.h"

extern Table *proc_table;

#pragma clang diagnostic ignored "-Wdeprecated-declarations"
Process *init_process(ProcessSerialNumber psn) {
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

Process *get_process(ProcessSerialNumber psn) {
    Process *process = (Process *)table_search(proc_table, psn.lowLongOfPSN);
    return process;
}

void remove_process(ProcessSerialNumber psn) {
    table_delete_item(proc_table, psn.lowLongOfPSN);
}

void get_process_list() {
    Process *process;
    ProcessSerialNumber psn = {kNoProcess, kNoProcess};

    while (GetNextProcess(&psn) == noErr) {
        process = init_process(psn);
        if (process != NULL) {
            if (!process->xpc && strcmp(process->name, "Finder") != 0 && strcmp(process->name, "Dock") != 0) {
                table_insert(proc_table, process->psn.lowLongOfPSN, (void *)process);
            } else {
                free(process);
            }
        }
    }
}
