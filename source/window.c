#include "include/window.h"

extern int g_connection;
extern Table *proc_table;
extern Table *window_table;
extern Table *app_table;
extern Table *space_table;

// returns index of displayList that has current display for given window
// void window_get_display(Window *w) {
//     uint32_t wid = w->wid;
//     CFStringRef displayString = SLSCopyManagedDisplayForWindow(g_connection, wid);
//     for (int i = 0; i < displayCount; i++) {
//         if (CFStringCompare(displayString, displayList[i].uuid, kCFCompareEqualTo) == 0) {
//             w->displayIndex = i;
//         }
//     }
//     CFRelease(displayString);
// }

void move_window_to_space(uint32_t wid, uint64_t sid) {
    CFArrayRef wids = cfarray_from_numbers(&wid, sizeof(int), 1, kCFNumberSInt32Type);
    SLSMoveWindowsToManagedSpace(g_connection, wids, sid);
    CFRelease(wids);
}

CGSize window_resize(Window *w, double width, double height) {
    CGSize newSize = {width, height};
    AXError err;
    CFTypeRef size = (CFTypeRef)(AXValueCreate(kAXValueCGSizeType, (const void *)&newSize));

    err = AXUIElementSetAttributeValue(w->uiElem, kAXSizeAttribute, size);
    CFRelease(size);
    return newSize;
}

void window_move_by_corner(Window *w, corner_t corner, double x, double y) {
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
    window_move(w, newPos.x, newPos.y);
}

Window *get_window(uint32_t wid) {
    Window *window;
    window = (Window *)table_search(window_table, wid);
    return window;
}

CGPoint window_move(Window *w, double x, double y) {
    CGPoint newPos = {x, y};
    AXError err;
    CFTypeRef pos = (CFTypeRef)(AXValueCreate(kAXValueCGPointType, (const void *)&newPos));

    err = AXUIElementSetAttributeValue(w->uiElem, kAXPositionAttribute, pos);
    CFRelease(pos);
    return newPos;
}

void window_get_size(Window *window) {
    CFTypeRef size;
    AXUIElementCopyAttributeValue(window->uiElem, kAXSizeAttribute, &size);
    AXValueGetValue(size, kAXValueCGSizeType, &window->size);
    CFRelease(size);
}

void window_get_position(Window *window) {
    CFTypeRef pos;
    AXUIElementCopyAttributeValue(window->uiElem, kAXPositionAttribute, &pos);
    AXValueGetValue(pos, kAXValueCGPointType, &window->position);
    CFRelease(pos);
}

void window_is_minimized(Window *window) {
    CFTypeRef mini;
    AXUIElementCopyAttributeValue(window->uiElem, kAXMinimizedAttribute, &mini);
    window->isMinimized = CFBooleanGetValue(mini);
    CFRelease(mini);
}

void window_get_dimensions(Window *w) {
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
pid_t get_window_owner(CFArrayRef window_ref, int ind) {
    int wid = get_cfnumber_from_array(window_ref, ind);
    int wcid; // window connection id
    pid_t pid;
    SLSGetWindowOwner(g_connection, wid, &wcid);
    SLSConnectionGetPID(wcid, &pid);
    return pid;
}

uint64_t current_space_for_window(Window *window) {
    int sid = 0;
    CFArrayRef window_list_ref = cfarray_from_numbers(&window->wid, sizeof(uint32_t), 1, kCFNumberSInt32Type);
    CFArrayRef space_list_ref = SLSCopySpacesForWindows(g_connection, 0x7, window_list_ref);
    int count = CFArrayGetCount(space_list_ref);
    if (count) {
        sid = get_cfnumber_from_array(space_list_ref, 0);
    }
    CFRelease(window_list_ref);
    CFRelease(space_list_ref);
    return (uint64_t)sid;
}

AXError set_focused_window(Application *application, uint32_t wid) {
    Window *window = get_window(wid);
    return AXUIElementSetAttributeValue(application->uiElem, kAXFocusedWindowAttribute, window->uiElem);
}

Window *get_window_from_axui(AXUIElementRef uiref) {
    uint32_t wid;
    _AXUIElementGetWindow(uiref, &wid);
    Window *w = get_window(wid);
    return w;
}

// dealing with array of window UIElemRef for the given app
static void iterate_app_windows(CFArrayRef app_windows, Application *app) {
    int count = CFArrayGetCount(app_windows);
    Application *tmp = *(&app);

    /**
     * if the app windows are processed for current space and then
     * there's another window on another space, the app will have an
     * increased window count. need to figure out how to avoid redundant processing.
     */
    if (count > app->windowCount && count > 0) {
        app->windowCount = count;
        if (tmp->wids == NULL) {
            tmp->wids = malloc(sizeof(uint32_t *) * count);
            for (int i = 0; i < count; i++) {
                AXUIElementRef wref = CFArrayGetValueAtIndex(app_windows, i);
                init_window(wref, app, i);
            }
        } else {
            tmp->wids = realloc(tmp->wids, sizeof(uint32_t *) * count);
            for (int i = 0; i < count; i++) {
                AXUIElementRef wref = CFArrayGetValueAtIndex(app_windows, i);
                init_window(wref, app, i);
            }
        }
    }
}

/**
 * get front app to not hide (avoids flashing)
 * for all other apps, hide them to interact and get AXUIElementRef
 * get owner pid for each window for the space
 */
static void init_windows(CFArrayRef space_windows, Space *space) {
    int count = CFArrayGetCount(space_windows);
    bool is_front_app = false;
    uint64_t cur_space = get_current_space();
    ProcessSerialNumber front_psn;
    _SLPSGetFrontProcess(&front_psn);
    Process *front_proc = get_process(front_psn);
    Application *front_app = get_application(front_proc->pid);

    // get owner pids for the wids in the space
    for (int i = 0; i < count; i++) {
        pid_t owner_pid = get_window_owner(space_windows, i);
        int wid = get_cfnumber_from_array(space_windows, i);
        Application *app = get_application(owner_pid);
        if (!app) {
            break;
        }
        if (space->sid != cur_space) {
            is_front_app = false;
            if (app->pid == front_app->pid) {
                move_window_to_space(wid, cur_space);
                is_front_app = true;
            } else {
                application_hide(app);
            }
        }
        CFArrayRef app_windows = get_application_windows(app);
        if (app_windows != NULL) {
            iterate_app_windows(app_windows, app);
        }
        if (is_front_app) {
            move_window_to_space(wid, space->sid);
        }
        application_unhide(app);
        CFRelease(app_windows);
    }
}

void get_window_list() {
    window_table->release = &release_window;
    Space *space = table_iterate(space_table, true);
    do {
        CFArrayRef space_windows = window_list_for_space(space->sid);
        init_windows(space_windows, space);
        CFRelease(space_windows);
    } while ((space = table_iterate(app_table, false)));
}

void window_callback(AXObserverRef observer, AXUIElementRef element, CFStringRef notifName, void *data) {
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
        window_get_size(window);
    }
}

AXError window_add_observers(Window *window) {
    Application *application = window->application;
    application_observe(application, window_callback);
    int count = arrayCount(notifs);
    AXError obser;

    for (int i = 0; i < count; i++) {
        CFStringRef notif = c_cfstring(notifs[i]);
        obser = AXObserverAddNotification(application->observer, application->uiElem, notif, window);
        CFRelease(notif);
    }
    return obser;
}

// window_list is axuielements of windows for the app
void init_window(AXUIElementRef window_uiref, Application *application, int wid_ind) {
    Window *window;
    window = malloc(sizeof(Window));
    window->uiElem = window_uiref;
    _AXUIElementGetWindow(window->uiElem, &window->wid);
    CFRetain(window->uiElem);
    window->application = application;
    window_add_observers(window);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), AXObserverGetRunLoopSource(application->observer), kCFRunLoopDefaultMode);
    window_get_size(window);
    window_get_position(window);
    window_is_minimized(window);
    // window_get_dimensions(window);

    /**
     * have to double check window table here because multiple windows for apps
     * will pass the fist check in get_window_list()
     */
    if (!get_window(window->wid)) {
        Application *tmp = *(&application);
        tmp->wids[wid_ind] = window->wid;
        table_insert(window_table, window->wid, window);
    } else {
        CFRelease(window->uiElem);
        free(window);
    }
}

void release_window(void *window) {
    Window *win = (Window *)window;
    CFRelease(win->uiElem);
}
