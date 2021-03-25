#include <objc/message.h>
#include <objc/runtime.h>
#include <stdio.h>

typedef struct NSType {
    Class class;
    id id;
    struct objc_object obj;
} NSType;

void ns_init(NSType *type, const char *class) {
    type->class = objc_getClass(class);
    type->obj.isa = type->class;
}

