#include "base.h"

typedef struct Window {
    int pid;
    char name[99];
    bool OnScren;
    int memoryUsage;
    CGPoint position;
    CGSize size;
} Window;

/*!
    @function getWindowList
    populates an array of Window structs for all currently visible windows
    @param Window pointer to Window struct. will initialize as array of Windows
*/
int getWindowList(Window **w);
