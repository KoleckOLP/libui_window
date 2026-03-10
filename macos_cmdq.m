// macos_cmdq.m
#import <AppKit/AppKit.h>

extern int request_app_exit(void *win);

void install_cmdq_monitor(void *win) {
    [NSEvent addLocalMonitorForEventsMatchingMask:NSEventMaskKeyDown
                                          handler:^NSEvent *(NSEvent *event) {
        NSEventModifierFlags mods =
            [event modifierFlags] & NSEventModifierFlagDeviceIndependentFlagsMask;
        NSString *chars = [event charactersIgnoringModifiers];

        if (mods == NSEventModifierFlagCommand &&
            chars != nil &&
            [chars caseInsensitiveCompare:@"q"] == NSOrderedSame) {
            if (request_app_exit(win)) {
                return nil;
            }
        }

        return event;
    }];
}
