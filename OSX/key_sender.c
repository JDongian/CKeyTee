// alterkeys.c
// http://osxbook.com
// https://gist.github.com/wewearglasses/8313195
//
// You need superuser privileges to create the event tap, unless accessibility
// is enabled. To do so, select the "Enable access for assistive devices"
// checkbox in the Universal Access system preference pane.

// TODO: add argv parsing
#define HOST "192.168.1.135"
#define PORT "35001"

#include <ApplicationServices/ApplicationServices.h>


void sendKeyInfo(CGEventType type, CGKeyCode keycode) {
    ; // Send type << 8 | keycode to HOST:PORT
}


CGEventRef keyCGEventCallback(CGEventTapProxy proxy, CGEventType type,
                             CGEventRef event, void *refcon) {
    // Paranoid sanity check.
    //if ((type != kCGEventKeyDown) && (type != kCGEventKeyUp))
    //    return event;

    // The incoming keycode.
    CGKeyCode keycode = (CGKeyCode)CGEventGetIntegerValueField(
            event, kCGKeyboardEventKeycode);

    if (type == kCGEventKeyUp) {
        printf("Sending KeyUp packet: %d\n", keycode);
    }
    if (type == kCGEventKeyDown) {
        printf("Sending KeyDown packet: %d\n", keycode);
    }
    sendKeyInfo(type, keycode);
    // Swap 'a' (keycode=0) and 'z' (keycode=6).
    //if (keycode == (CGKeyCode)0)
    //    keycode = (CGKeyCode)6;
    //else if (keycode == (CGKeyCode)6)
    //    keycode = (CGKeyCode)0;

    // Set the modified keycode field in the event.
    CGEventSetIntegerValueField(
            event, kCGKeyboardEventKeycode, (int64_t)keycode);

    // We must return the event for it to be useful.
    return event;
}

int main(void) {
    CFMachPortRef eventTap;
    CGEventMask eventMask;
    CFRunLoopSourceRef runLoopSource;

    // Create an event tap. We are interested in key presses.
    eventMask = ((1 << kCGEventKeyDown) | (1 << kCGEventKeyUp));
    eventTap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, 0,
            eventMask, keyCGEventCallback, NULL);
    if (!eventTap) {
        fprintf(stderr, "failed to create event tap\n");
        exit(1);
    }

    // Create a run loop source.
    runLoopSource = CFMachPortCreateRunLoopSource(
            kCFAllocatorDefault, eventTap, 0);

    // Add to the current run loop.
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource,
            kCFRunLoopCommonModes);

    // Enable the event tap.
    CGEventTapEnable(eventTap, true);

    // Set it all running.
    CFRunLoopRun();

    // In a real program, one would have arranged for cleaning up.

    exit(0);
}
