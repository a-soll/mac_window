#include "../include/nsworkspace.h"
#include <objc/message.h>
#include <objc/runtime.h>

void ws_init(NSWorkspace *ws) {
    ws->class = objc_getClass("NSWorkspace");
    ws->obj.isa = ws->class;
}
