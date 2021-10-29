#include "include/window.h"

extern int g_connection;
extern Table *proc_table;
extern Table *window_table;
extern Table *app_table;
extern Table *space_table;

// returns index of displayList that has current display for given window
// void windowGetDisplay(Window *w) {
//     uint32_t wid = w->wid;
//     CFStringRef displayString = SLSCopyManagedDisplayForWindow(g_connection, wid);
//     for (int i = 0; i < displayCount; i++) {
//         if (CFStringCompare(displayString, displayList[i].uuid, kCFCompareEqualTo) == 0) {
//             w->displayIndex = i;
//         }
//     }
//     CFRelease(displayString);
// }

void moveWindowToSpace(uint32_t wid, uint64_t sid) {
    CFArrayRef wids = CFArrayFromNumbers(&wid, sizeof(int), 1, kCFNumberSInt32Type);
    SLSMoveWindowsToManagedSpace(g_connection, wids, sid);
    CFRelease(wids);
}

CGSize windowResize(Window *w, double width, double height) {
    CGSize newSize = {width, height};
    AXError err;
    CFTypeRef size = (CFTypeRef)(AXValueCreate(kAXValueCGSizeType, (const void *)&newSize));

    err = AXUIElementSetAttributeValue(w->uiElem, kAXSizeAttribute, size);
    CFRelease(size);
    return newSize;
}

void windowMoveByCorner(Window *w, corner_t corner, double x, double y) {
    CGPoint newPos;
    double x_diff;
    double y_diff;

    switch (corner) {
    case cTopLeft:
        newPos.x = x;
        newPos.y = y;
        break;
    case cTopRight:
        x_diff = x - w->topright.x;
        newPos.x = x_diff + w->topleft.x;
        newPos.y = y;
        break;
    case cBottomLeft:
        newPos.x = x;
        newPos.y = y - w->size.height;
        break;
    case cBottomRight:
        x_diff = w->bottomright.x - x;
        newPos.x = (w->bottomright.x - x_diff) - w->size.width;
        y_diff = w->size.height + (w->bottomright.y - y);
        newPos.y = w->bottomright.y - y_diff;
        break;
    default:
        newPos.x = x;
        newPos.y = y;
        break;
    }
    windowMove(w, newPos.x, newPos.y);
}

Window *getWindow(uint32_t wid) {
    Window *window;
    window = table_search(window_table, wid);
    return window;
}

CGPoint windowMove(Window *w, double x, double y) {
    CGPoint newPos = {x, y};
    AXError err;
    CFTypeRef pos = (CFTypeRef)(AXValueCreate(kAXValueCGPointType, (const void *)&newPos));

    err = AXUIElementSetAttributeValue(w->uiElem, kAXPositionAttribute, pos);
    CFRelease(pos);
    return newPos;
}

void windowGetSize(Window *window) {
    CFTypeRef size;
    AXUIElementCopyAttributeValue(window->uiElem, kAXSizeAttribute, &size);
    AXValueGetValue(size, kAXValueCGSizeType, &window->size);
    CFRelease(size);
}

void windowGetPosition(Window *window) {
    CFTypeRef pos;
    AXUIElementCopyAttributeValue(window->uiElem, kAXPositionAttribute, &pos);
    AXValueGetValue(pos, kAXValueCGPointType, &window->position);
    CFRelease(pos);
}

void windowIsMinimized(Window *window) {
    CFTypeRef mini;
    AXUIElementCopyAttributeValue(window->uiElem, kAXMinimizedAttribute, &mini);
    window->isMinimized = CFBooleanGetValue(mini);
    CFRelease(mini);
}

void windowGetDimensions(Window *w) {
    w->topleft.x = w->position.x;
    w->topleft.y = w->position.y;

    w->topright.x = w->position.x + w->size.width;
    w->topright.y = w->position.y;

    w->bottomleft.x = w->position.x;
    w->bottomleft.y = w->topleft.y + w->size.height;

    w->bottomright.x = w->position.x + w->size.width;
    w->bottomright.y = w->bottomleft.y;
}

// get pid of owner for wid in array
pid_t getWindowOwner(CFArrayRef window_ref, int ind) {
    int wid = getNumberFromArray(window_ref, ind);
    int wcid; // window connection id
    pid_t pid;
    SLSGetWindowOwner(g_connection, wid, &wcid);
    SLSConnectionGetPID(wcid, &pid);
    return pid;
}

uint64_t currentSpaceForWindow(Window *window) {
    int sid = 0;
    CFArrayRef window_list_ref = CFArrayFromNumbers(&window->wid, sizeof(uint32_t), 1, kCFNumberSInt32Type);
    CFArrayRef space_list_ref = SLSCopySpacesForWindows(g_connection, 0x7, window_list_ref);
    int count = CFArrayGetCount(space_list_ref);
    if (count) {
        sid = getNumberFromArray(space_list_ref, 0);
    }
    CFRelease(window_list_ref);
    CFRelease(space_list_ref);
    return (uint64_t)sid;
}

AXError setFocusedWindow(Application *application, uint32_t wid) {
    Window *window = table_search(window_table, wid);
    return AXUIElementSetAttributeValue(application->uiElem, kAXFocusedWindowAttribute, window->uiElem);
}

void getWindowList() {
    window_table->release = &releaseWindow;
    uint64_t cur_space = getActiveSpace();
    ProcessSerialNumber front_psn;
    _SLPSGetFrontProcess(&front_psn);
    Process *front_proc = table_search(proc_table, front_psn.lowLongOfPSN);
    Application *front_app = table_search(app_table, front_proc->pid);
    uint32_t front_wid = getApplicationFocusedWindow(front_app);

    // get list of windows for each space
    for (int j = 0; j < space_table->size; j++) {
        if (valid_bucket(space_table, j)) {
            Space *space = (Space *)space_table->buckets[j]->data;
            CFArrayRef window_ref;
            window_ref = spaceWindows(space->sid);
            int count = CFArrayGetCount(window_ref);

            // get owner pid for each window
            for (int i = 0; i < count; i++) {
                pid_t pid = getWindowOwner(window_ref, i);
                int wid = getNumberFromArray(window_ref, i);
                Application *application = (Application *)table_search(app_table, pid);
                if (!application) {
                    break;
                }
                CFArrayRef app_windows;
                /**In order to create AXUIElement for windows from other spaces,
                 * we need to interact with the process by invoking hide. we then unhide it.
                 * To avoid flashing the currently focused app, we simply move the windows from other
                 * spaces to the current one.
                 */
                if (space->sid != cur_space) {
                    bool cur_app = false;
                    if (application->pid == front_app->pid) {
                        moveWindowToSpace(wid, cur_space);
                        cur_app = true;
                    } else {
                        applicationHide(application);
                    }
                    app_windows = getApplicationWindows(application);
                    if (!table_search(window_table, wid)) {
                        initWindow(app_windows, application);
                    }
                    applicationUnHide(application);
                    if (cur_app) {
                        moveWindowToSpace(wid, space->sid);
                    }
                } else {
                    app_windows = getApplicationWindows(application);
                    if (app_windows != NULL && !table_search(window_table, wid)) {
                        initWindow(app_windows, application);
                    }
                }
                CFRelease(app_windows);
            }
            CFRelease(window_ref);
        }
    }
}

void windowCallback(AXObserverRef observer, AXUIElementRef element, CFStringRef notifName, void *data) {
    Window *window = data;

    if (CFEqual(kAXWindowMiniaturizedNotification, notifName)) {
        window->isMinimized = true;
    }
    if (CFEqual(kAXWindowDeminiaturizedNotification, notifName)) {
        window->isMinimized = false;
    }
    if (CFEqual(kAXResizedNotification, notifName)) {
        // placeholder
    }
    if (CFEqual(kAXWindowResizedNotification, notifName)) {
        windowGetSize(window);
    }
}

void windowAddObservers(Window *window) {
    Application *application = window->application;
    int count = arrayCount(notifs);
    AXError err = AXObserverCreate(application->pid, windowCallback, &application->observer);

    for (int i = 0; i < count; i++) {
        CFStringRef notif = CCFSTRING(notifs[i]);
        AXError obser = AXObserverAddNotification(application->observer, application->uiElem, notif, window);
        CFRelease(notif);
    }
}

// window_list is axuielements of windows for the app
void initWindow(CFArrayRef window_list, Application *application) {
    CFIndex c = CFArrayGetCount(window_list);

    for (int i = 0; i < c; i++) {
        Window *window;
        window = malloc(sizeof(Window));
        window->uiElem = CFArrayGetValueAtIndex(window_list, i);
        _AXUIElementGetWindow(window->uiElem, &window->wid);
        CFRetain(window->uiElem);
        window->application = application;
        windowAddObservers(window);
        CFRunLoopAddSource(CFRunLoopGetCurrent(), AXObserverGetRunLoopSource(application->observer), kCFRunLoopDefaultMode);
        windowGetSize(window);
        windowGetPosition(window);
        windowIsMinimized(window);
        // windowGetDimensions(window);

        /**have to double check window table here because multiple windows for apps
         * will pass the fist check in getWindowList()
         */
        if (!table_search(window_table, window->wid)) {
            table_insert(window_table, window->wid, window);
        }
        else {
            CFRelease(window->uiElem);
            free(window);
        }
    }
}

void releaseWindow(void *window) {
    Window *win = (Window *)window;
    CFRelease(win->uiElem);
}
