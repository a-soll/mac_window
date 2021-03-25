#include <objc/message.h>
#include <objc/runtime.h>
#include <stdio.h>
#include "include/nsworkspace.h"
#include "include/nsarray.h"

int main() {
    NSWorkspace ws;
    ws.class = objc_getClass("NSWorkspace");
    struct objc_object ws_obj = {ws.class};
    ws.id = ((id (*)(Class, SEL))objc_msgSend)(ws.class, sel_registerName("sharedWorkspace"));

    NSArray apps;
    apps.class = objc_getClass("NSArray");
    struct objc_object apps_obj = {apps.class};
    apps.id = ((id (*)(id, SEL))objc_msgSend)(ws.id, sel_registerName("runningApplications"));

    Class app = objc_getClass("NSRunningApplication");

    int count;
    count = ((int (*)(id, SEL))objc_msgSend)(apps.id, sel_registerName("count"));

    printf("%d\n", count);
}
