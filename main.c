#include "precompiled.h"
#include <objc/message.h>
#include <objc/runtime.h>
#include <stdio.h>

// TODO: a lot
int main() {
    NSWorkspace ws;
    ws_init(&ws);
    SEL workspace = sel_registerName("sharedWorkspace");
    ws.id = ((id(*)(Class, SEL))objc_msgSend)(ws.class, workspace);

    NSArray apps;
    nsarr_init(&apps);
    SEL running_apps = sel_registerName("runningApplications");
    apps.id = ((id(*)(id, SEL))objc_msgSend)(ws.id, running_apps);

    int count;
    SEL apps_count = sel_registerName("count");
    count = ((int (*)(id, SEL))objc_msgSend)(apps.id, apps_count);

    NSRunningApplication app;
    nsrunningapp_init(&app);

    for (unsigned long i = 0; i < count; i++) {
        NSString p_name;
        nsstr_init(&p_name);

        SEL index = sel_registerName("objectAtIndex:");
        app.id = ((id(*)(id, SEL, unsigned long))objc_msgSend)(apps.id, index, i);

        SEL proc_ids = sel_registerName("processIdentifier");
        pid_t pid = ((pid_t(*)(id, SEL))objc_msgSend)(app.id, proc_ids);

        SEL proc_name = sel_registerName("localizedName");
        p_name.id = ((id(*)(id, SEL))objc_msgSend)(app.id, proc_name);
        c_string(&p_name);

        printf("pid: %d    %s\n", (int)pid, p_name.c_string);
    }
}
