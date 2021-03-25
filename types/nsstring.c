#include "../include/nsstring.h"
#include <objc/message.h>
#include <objc/runtime.h>

void nsstr_init(NSString *nstr) {
    nstr->class = objc_getClass("NSString");
    nstr->obj.isa = nstr->class;
}

const char *c_string(NSString *nstr) {
    SEL m = sel_registerName("UTF8String");
    const char *ret = ((const char *(*)(id, SEL))objc_msgSend)(nstr->id, m);
    return ret;
}
