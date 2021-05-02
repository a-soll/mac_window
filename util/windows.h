#include "base.h"
#include <ApplicationServices/ApplicationServices.h>

typedef struct Window {
    pid_t pid;
    char name[99];
    bool onScreen;
    CGPoint position;
    CGSize size;
    CFArrayRef uiElements;
} Window;

/*!
    @function getWindowList
    populates an array of Window structs for all currently visible windows
    @param Window pointer to Window struct. will initialize as array of Windows
*/
int getWindowList(Window **w);

/*!
    @function getWindowByName
    returns index of given app
    @param Window the list of Windows
    @param count length of the Window array
    @param appName name of app to search for
*/
int getWindowByName(Window *w, int count, const char *appName);

// move the app given app window
void moveWindow(Window *w, int x, int y);

// properly free Window list
void releaseWindow(Window *w, int count);
