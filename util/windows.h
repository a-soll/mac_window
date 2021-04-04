#include "base.h"

typedef struct Window {
    int pid;
    const char *name;
    bool OnScren;
    int memoryUsage;
    struct windowBounds {
        int height;
        int width;
        int x;
        int y;
    } windowBounds;
} Window;

void getWindowByName(Window *w, const char *name);
