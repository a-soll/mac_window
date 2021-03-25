#include "../include/nsrunningapp.h"
#include <objc/message.h>
#include <objc/runtime.h>

void nsrunningapp_init(NSRunningApplication *type) {
    type->class = objc_getClass("NSRunningApplication");
    type->obj.isa = type->class;
}
