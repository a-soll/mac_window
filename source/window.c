#include "include/window.h"

extern int g_connection;
extern Table *proc_table;
extern Table *window_table;
extern Table *app_table;

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

// TODO: this will create windows and applications from the processes
void getWindowList() {
    Application *application;
    window_table->release = &releaseWindow;

    int i = 0;
    while (i < proc_table->size) {
        if (valid_bucket(proc_table, i)) {
            CFArrayRef window_list;
            Process *process = (Process *)proc_table->buckets[i]->data;
            application = initApplication(process);
            window_list = getApplicationWindows(application);
            if (window_list != NULL) {
                initWindow(window_list, application);
                CFRelease(window_list);
            }
        }
        i++;
    }
}

// TODO: move the window that's touching the moving window's edge
void windowIsResizing(AXObserverRef observer, AXUIElementRef element, CFStringRef notifName, void *contextData) {
    Window *window = (Window *)contextData;
}

void initWindow(CFArrayRef window_list, Application *application) {
    CFTypeRef size;
    CFTypeRef pos;
    CFIndex c = CFArrayGetCount(window_list);

    for (int i = 0; i < c; i++) {
        Window *window;
        window = malloc(sizeof(Window));
        window->uiElem = CFArrayGetValueAtIndex(window_list, i);
        CFRetain(window->uiElem);
        window->application = application;
        AXError err = AXObserverCreate(application->pid, windowIsResizing, &application->observer);
        AXError obser = AXObserverAddNotification(application->observer, application->uiElem, kAXResizedNotification, window);
        CFRunLoopAddSource(CFRunLoopGetCurrent(), AXObserverGetRunLoopSource(application->observer), kCFRunLoopDefaultMode);
        AXUIElementCopyAttributeValue(window->uiElem, kAXSizeAttribute, &size);
        AXUIElementCopyAttributeValue(window->uiElem, kAXPositionAttribute, &pos);
        _AXUIElementGetWindow(window->uiElem, &window->wid);
        AXValueGetValue(size, kAXValueCGSizeType, &window->size);
        AXValueGetValue(pos, kAXValueCGPointType, &window->position);
        // windowGetDisplay(window);
        // windowGetDimensions(window);
        table_insert(window_table, window->wid, window);
        CFRelease(size);
        CFRelease(pos);
    }
}

void releaseWindow(void *window) {
    Window *win = (Window *)window;
    CFRelease(win->uiElem);
}
