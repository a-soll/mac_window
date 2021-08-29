#include "include/window.h"

extern int g_connection;
extern Table *proc_table;

// returns index of displayList that has current display for given window
void windowGetDisplay(Window *w) {
    uint32_t wid = w->wid;
    CFStringRef displayString = SLSCopyManagedDisplayForWindow(g_connection, wid);
    for (int i = 0; i < displayCount; i++) {
        if (CFStringCompare(displayString, displayList[i].uuid, kCFCompareEqualTo) == 0) {
            w->displayIndex = i;
        }
    }
    CFRelease(displayString);
}

void windowResize(Window *w, double width, double height) {
    CGSize newSize = {width, height};
    AXError err;
    CFTypeRef size = (CFTypeRef)(AXValueCreate(kAXValueCGSizeType, (const void *)&newSize));

    err = AXUIElementSetAttributeValue(w->uiElem, kAXSizeAttribute, size);
    if (err == kAXErrorSuccess) {
        w->size = newSize;
    }
    CFRelease(size);
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

void windowMove(Window *w, double x, double y) {
    CGPoint newPos = {x, y};
    AXError err;
    CFTypeRef pos = (CFTypeRef)(AXValueCreate(kAXValueCGPointType, (const void *)&newPos));

    err = AXUIElementSetAttributeValue(w->uiElem, kAXPositionAttribute, pos);
    if (err == kAXErrorSuccess) {
        w->position = newPos;
        windowGetDisplay(w);
    }
    CFRelease(pos);
}

void windowGetDimensions(Window *w) {
    Display *d = &displayList[w->displayIndex];

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
    Window *window;

    int i = 0;
    while (i < proc_table->size) {
        if (valid_bucket(proc_table, i)) {
            Process *process = (Process *)proc_table->buckets[i]->data;
            printf("%s\n", process->name);
            printf("%d\n", process->pid);
            printf("%d\n", process->psn.lowLongOfPSN);
        }
        i++;
    }
}

// for (int i = 0; i < proc_table->size; i++) {
//     if (proc_table->buckets[i] != NULL) {
//         Process *process = (Process *)table_search(proc_table, proc_table->buckets[i]->key);
//         if (process != NULL) {
//             printf("%s\n", process->name);
//             printf("%d\n", process->pid);
//             printf("%d\n", process->psn.lowLongOfPSN);
//         }
//     }
// }

// app.uiElem = AXUIElementCreateApplication(pid);
// CFArrayRef window_list = getApplicationWindows(app);
// CFShow(window_list);
// CFArrayRef ws = CGWindowListCreate(kCGWindowListExcludeDesktopElements, kCGNullWindowID);
// CFIndex count = CFArrayGetCount(ws);
// CFArrayRef ws_info = CGWindowListCreateDescriptionFromArray(ws);

// for (int i = 0; i < count; i++) {
//     CFDictionaryRef info = (CFTypeRef)CFArrayGetValueAtIndex(ws_info, i);
//     CFShow(info);
// }
// }

void initWindow(Window *w) {
    CFTypeRef size;
    CFTypeRef pos;
    uint32_t wid;

    AXUIElementRef elem = AXUIElementCreateApplication(w->application->pid);
    CFShow(elem);
    // w->uiElem = CFArrayGetValueAtIndex(uiElems, i);
    // AXUIElementCopyAttributeValue(w[i].uiElem, kAXSizeAttribute, &size);
    // AXUIElementCopyAttributeValue(w[i].uiElem, kAXPositionAttribute, &pos);
    // // retain uiElem so that uiElems can be freed without losing reference
    // CFRetain(w[i].uiElem);
    // AXValueGetValue(size, kAXValueCGSizeType, &w[i].size);
    // AXValueGetValue(pos, kAXValueCGPointType, &w[i].position);
    // _AXUIElementGetWindow(w[i].uiElem, &wid);
    // w[i].wid = wid;
    // windowGetDisplay(&w[i]);
    // windowGetDimensions(&w[i]);
    // CFRelease(size);
    // CFRelease(pos);
}

void releaseWindowList(Window *w, int count) {
    for (int i = 0; i < count; i++) {
        if (w[i].uiElem) {
            CFRelease(w[i].uiElem);
        }
    }
    free(w);
}
