#include <objc/runtime.h>
#include <objc/message.h>


typedef struct NSArray {
    Class class;
    id id;
    struct objc_object obj;
} NSArray;

void nsarr_init(NSArray *arr);

// ---------------------------------------------------

typedef struct NSRunningApplication {
    Class class;
    id id;
    struct objc_object obj;
} NSRunningApplication;

void nsrunningapp_init(NSRunningApplication *type);

// ---------------------------------------------------

typedef struct NSString {
    Class class;
    id id;
    struct objc_object obj;
    const char *c_string;
} NSString;

void nsstr_init(NSString *nstr);

void c_string(NSString *nstr);

// ---------------------------------------------------

typedef struct NSWorkspace {
    Class class;
    id id;
    struct objc_object obj;
    SEL sel;
} NSWorkspace;

void ws_init(NSWorkspace *ws);

