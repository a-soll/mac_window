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
    CopyProcessName(&psn, &name_ref);
    process = malloc(sizeof(Process));
    CFStringGetCString(name_ref, process->name, APP_NAME_MAX, kUnicodeUTF8Format);
    process->pid = pid;
    process->psn = psn;
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
    proc_table = table_init(125);

    ProcessSerialNumber psn = {kNoProcess, kNoProcess};
    while (GetNextProcess(&psn) == noErr) {
        process = initProcess(psn);
        table_insert(proc_table, process->psn.lowLongOfPSN, process);
    }
}
