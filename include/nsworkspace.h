#include <objc/runtime.h>
#include <objc/message.h>

typedef struct NSWorkspace {
    Class class;
    id id;
    struct objc_object obj;
    SEL sel;
} NSWorkspace;

void ws_init(NSWorkspace *ws, const char* init_m) {
    ws->class = objc_getClass("NSWorkspace");
    ws->obj.isa = ws->class;
    ws->id = ((id(*)(Class, SEL))objc_msgSend)(ws->class, sel_registerName(init_m));
}
