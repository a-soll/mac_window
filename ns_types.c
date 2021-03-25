#include "include/ns_types.h"
#include <objc/message.h>
#include <objc/runtime.h>


void nsarr_init(NSArray *nsarr) {
    nsarr->class = objc_getClass("NSArray");
    nsarr->obj.isa = nsarr->class;
}

// ---------------------------------------------------

void nsrunningapp_init(NSRunningApplication *type) {
    type->class = objc_getClass("NSRunningApplication");
    type->obj.isa = type->class;
}

// ---------------------------------------------------

void nsstr_init(NSString *nstr) {
    nstr->class = objc_getClass("NSString");
    nstr->obj.isa = nstr->class;
}

void c_string(NSString *nstr) {
    SEL m = sel_registerName("UTF8String");
    nstr->c_string = ((const char *(*)(id, SEL))objc_msgSend)(nstr->id, m);
}

// ---------------------------------------------------

void ws_init(NSWorkspace *ws) {
    ws->class = objc_getClass("NSWorkspace");
    ws->obj.isa = ws->class;
}
