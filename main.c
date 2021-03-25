#include "include/nsarray.h"
#include "include/nsworkspace.h"
#include <objc/message.h>
#include <objc/runtime.h>
#include <stdio.h>

int main() {
    NSWorkspace ws;
    ws.class = objc_getClass("NSWorkspace");
    struct objc_object ws_obj = {ws.class};
    ws.id = ((id(*)(Class, SEL))objc_msgSend)(ws.class, sel_registerName("sharedWorkspace"));

    NSArray apps;
    apps.class = objc_getClass("NSArray");
    struct objc_object apps_obj = {apps.class};
    apps.id = ((id(*)(id, SEL))objc_msgSend)(ws.id, sel_registerName("runningApplications"));

    Class app = objc_getClass("NSRunningApplication");
    struct objc_object app_obj = {app};
    id app_id;

    int count;
    count = ((int (*)(id, SEL))objc_msgSend)(apps.id, sel_registerName("count"));

    for (unsigned long i = 0; i < count; i++) {
        app_id = ((id(*)(id, SEL, unsigned long))objc_msgSend)(apps.id, sel_registerName("objectAtIndex:"), i);
        pid_t pid = ((pid_t(*)(id, SEL))objc_msgSend)(app_id, sel_registerName("processIdentifier"));
        printf("%ld\n", (long)pid);
    }
}
