#include <objc/runtime.h>
#include <objc/message.h>

typedef struct NSString {
    Class class;
    id id;
    struct objc_object obj;
} NSString;

void nsstr_init(NSString *nstr);

const char* c_string(NSString *nstr);
