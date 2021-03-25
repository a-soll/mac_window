#include "../include/nsarray.h"
#include <objc/message.h>
#include <objc/runtime.h>

void nsarr_init(NSArray *nsarr) {
    nsarr->class = objc_getClass("NSArray");
    nsarr->obj.isa = nsarr->class;
}
