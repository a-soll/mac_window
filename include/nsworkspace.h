#include <objc/runtime.h>
#include <objc/message.h>

typedef struct NSWorkspace {
    Class class;
    id id;
    struct objc_object obj;
    SEL sel;
} NSWorkspace;

void ws_init(NSWorkspace *ws);
