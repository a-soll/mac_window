#include <objc/message.h>
#include <objc/runtime.h>
#include <stdio.h>

typedef struct NSRunningApplication {
    Class class;
    id id;
    struct objc_object obj;
} NSRunningApplication;

void nsrunningapp_init(NSRunningApplication *type);
