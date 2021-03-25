#include <objc/message.h>
#include <objc/runtime.h>

typedef struct NSString {
    Class class;
    id id;
    struct objc_object obj;
    const char *c_string;
} NSString;

void nsstr_init(NSString *nstr);

void c_string(NSString *nstr);
