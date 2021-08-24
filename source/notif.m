#import <AppKit/AppKit.h>
#import "include/notif.h"

@interface workspace_context : NSObject
- (id) init;
- (void)activeSpaceDidChange:(NSNotification*)inNotification;
@end

@implementation workspace_context

- (id) init
{
    if((self = [super init])) {
    [[[NSWorkspace sharedWorkspace] notificationCenter]
        addObserver:self
        selector:@selector(activeSpaceDidChange:)
        name:NSWorkspaceActiveSpaceDidChangeNotification
        object:nil];
    }
    return self;
}

- (void) activeSpaceDidChange:(NSNotification *)notification
{
    spaceDidChange([notification.name UTF8String]);
}

@end

void workspace_event_handler_init(void **context)
{
    workspace_context *ws_context = [workspace_context alloc];
    *context = ws_context;
}

void workspace_event_handler_begin(void **context)
{
    workspace_context *ws_context = *context;
    [ws_context init];
}

bool NSAppLoad(void) {
    NSLog(@"loading");
    return NSApplicationLoad();
}
