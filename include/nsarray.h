#include <objc/runtime.h>
#include <objc/message.h>

typedef struct NSArray {
    Class class;
    id id;
    struct objc_object obj;
} NSArray;

void nsarr_init(NSArray *arr);
