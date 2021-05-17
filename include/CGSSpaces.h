#ifndef CGSSPACES_H
#define CGSSPACES_H

#include <CoreGraphics/CoreGraphics.h>
#include <ApplicationServices/ApplicationServices.h>
#include <CoreFoundation/CoreFoundation.h>

#define kCGSIgnoreForExposeTagBit (1 << 7)
#define kCGSIgnoreForEventsTagBit (1 << 9)
#define kCGSDisableShadowTagBit   (1 << 3)

// const CFStringRef kAXEnhancedUserInterface = CFSTR("AXEnhancedUserInterface");

// #define AX_ENHANCED_UI_WORKAROUND(r, c) \
// {\
//     bool eui = ax_enhanced_userinterface(r); \
//     if (eui) AXUIElementSetAttributeValue(r, kAXEnhancedUserInterface, kCFBooleanFalse); \
//     c \
//     if (eui) AXUIElementSetAttributeValue(r, kAXEnhancedUserInterface, kCFBooleanTrue); \
// }
CGDirectDisplayID convertUUIDtoID(char* uuid);
void CGSConfigureDisplayMode(CGDisplayConfigRef config, CGDirectDisplayID display, int modeNum);
void CGSGetCurrentDisplayMode(CGDirectDisplayID display, int* modeNum);
extern uint64_t SLSGetActiveSpace(int cid);
extern AXError _AXUIElementGetWindow(AXUIElementRef ref, uint32_t *wid);
extern int SLSMainConnectionID(void);
extern CGError SLSGetWindowBounds(int cid, uint32_t wid, CGRect *frame);
extern CGError SLSGetWindowLevel(int cid, uint32_t wid, int *level);
extern CGError SLSGetWindowAlpha(int cid, uint32_t wid, float *alpha);
extern CGError SLSSetWindowResolution(int cid, uint32_t wid, double resolution);
extern CGError SLSCopyWindowProperty(int cid, uint32_t wid, CFStringRef property, CFTypeRef *value);
extern CFStringRef SLSCopyManagedDisplayForWindow(int cid, uint32_t wid);
extern CFStringRef SLSCopyBestManagedDisplayForRect(int cid, CGRect rect);
extern CFArrayRef SLSCopySpacesForWindows(int cid, int selector, CFArrayRef window_list);
extern CGError SLSDisableUpdate(int cid);
extern CGError SLSReenableUpdate(int cid);
extern CGError SLSNewWindow(int cid, int type, float x, float y, CFTypeRef region, uint32_t *wid);
extern CGError SLSReleaseWindow(int cid, uint32_t wid);
extern CGError SLSSetWindowTags(int cid, uint32_t wid, uint64_t *tags, int tag_size);
extern CGError SLSSetWindowShape(int cid, uint32_t wid, float x_offset, float y_offset, CFTypeRef shape);
extern CGError SLSSetWindowOpacity(int cid, uint32_t wid, bool opaque);
extern CGError SLSOrderWindow(int cid, uint32_t wid, int mode, uint32_t rel_wid);
extern CGError SLSSetWindowLevel(int cid, uint32_t wid, int level);
extern CGContextRef SLWindowContextCreate(int cid, uint32_t wid, CFDictionaryRef options);
extern CGError CGSNewRegionWithRect(CGRect *rect, CFTypeRef *region);
extern CFUUIDRef CGDisplayCreateUUIDFromDisplayID(uint32_t did);
extern CFArrayRef SLSCopyManagedDisplays(int cid);
extern uint64_t SLSManagedDisplayGetCurrentSpace(int cid, CFStringRef uuid);
extern CFStringRef SLSCopyActiveMenuBarDisplayIdentifier(int cid);
extern CFStringRef SLSCopyBestManagedDisplayForPoint(int cid, CGPoint point);
extern bool SLSManagedDisplayIsAnimating(int cid, CFStringRef uuid);
extern CGError SLSGetMenuBarAutohideEnabled(int cid, int *enabled);
extern CGError SLSGetRevealedMenuBarBounds(CGRect *rect, int cid, uint64_t sid);
extern CGError SLSGetDockRectWithReason(int cid, CGRect *rect, int *reason);
extern Boolean CoreDockGetAutoHideEnabled(void);
extern void CoreDockGetOrientationAndPinning(int *orientation, int *pinning);
extern CFStringRef SLSCopyManagedDisplayForSpace(int cid, uint64_t sid);
extern int SLSSpaceGetType(int cid, uint64_t sid);
extern CFStringRef SLSSpaceCopyName(int cid, uint64_t sid);
extern CFArrayRef SLSCopyWindowsWithOptionsAndTags(int cid, uint32_t owner, CFArrayRef spaces, uint32_t options, uint64_t *set_tags, uint64_t *clear_tags);
extern int SLSGetSpaceManagementMode(int cid);
extern CFArrayRef SLSCopyManagedDisplaySpaces(int cid);
extern CGError SLSProcessAssignToSpace(int cid, pid_t pid, uint64_t sid);
extern CGError SLSProcessAssignToAllSpaces(int cid, pid_t pid);
extern void SLSMoveWindowsToManagedSpace(int cid, CFArrayRef window_list, uint64_t sid);
extern CGError CoreDockSendNotification(CFStringRef notification, int unknown);
extern CGError SLSMoveWindow(int cid, uint32_t wid, CGPoint *point);
extern CFArrayRef SLSCopyAssociatedWindows(int cid, uint32_t wid);
extern CFTypeRef SLSWindowQueryWindows(int cid, CFArrayRef windows, int count);
extern CFTypeRef SLSWindowQueryResultCopyWindows(CFTypeRef window_query);
extern CGError SLSWindowIteratorAdvance(CFTypeRef iterator);
extern uint32_t SLSWindowIteratorGetParentID(CFTypeRef iterator);
extern uint32_t SLSWindowIteratorGetWindowID(CFTypeRef iterator);
extern uint64_t SLSWindowIteratorGetTags(CFTypeRef iterator);
extern OSStatus _SLPSGetFrontProcess(ProcessSerialNumber *psn);
extern CGError SLSGetWindowOwner(int cid, uint32_t wid, int *wcid);
extern CGError SLSGetConnectionPSN(int cid, ProcessSerialNumber *psn);
extern CGError SLSGetConnectionIDForPSN(int cid, ProcessSerialNumber *psn, int *process_cid);
extern CGError SLSConnectionGetPID(int cid, pid_t *pid);
extern CGError _SLPSSetFrontProcessWithOptions(ProcessSerialNumber *psn, uint32_t wid, uint32_t mode);
extern CGError SLPSPostEventRecordTo(ProcessSerialNumber *psn, uint8_t *bytes);
extern OSStatus SLSFindWindowByGeometry(int cid, int zero, int one, int zero_again, CGPoint *screen_point, CGPoint *window_point, uint32_t *wid, int *wcid);
extern CGError SLSGetCurrentCursorLocation(int cid, CGPoint *point);

#if 0
extern CFArrayRef _LSCopyApplicationArrayInFrontToBackOrder(int negative_one, int one);
extern void _LSASNExtractHighAndLowParts(const void *asn, uint32_t *high, uint32_t *low);
extern CFTypeID _LSASNGetTypeID(void);
#endif

#endif /* CGSSPACES_H */
