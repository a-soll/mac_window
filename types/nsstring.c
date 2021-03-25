#include "../include/nsstring.h"
#include <objc/message.h>
#include <objc/runtime.h>

void nsstr_init(NSString *nstr) {
    nstr->class = objc_getClass("NSString");
    nstr->obj.isa = nstr->class;
}

void c_string(NSString *nstr) {
    SEL m = sel_registerName("UTF8String");
    nstr->c_string = ((const char *(*)(id, SEL))objc_msgSend)(nstr->id, m);
}
