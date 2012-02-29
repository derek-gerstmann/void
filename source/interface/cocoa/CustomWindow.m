#import "CustomWindow.h"

static int keyUpArrow = 0;
static int keyDownArrow = 0;
static int keyDelete = 0;

@implementation CustomWindow

@synthesize initialLocation;

/*
 In Interface Builder, the class for the window is set to this subclass. Overriding the initializer provides a mechanism for controlling how objects of this class are created.
 */
- (id)initWithContentRect:(NSRect)contentRect styleMask:(NSUInteger)aStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)flag
{

    NSRect screenVisibleFrame = [[NSScreen mainScreen] visibleFrame];

    contentRect = NSMakeRect(
        100,
        screenVisibleFrame.origin.y + (screenVisibleFrame.size.height - vdCocoaGetWindowConfig().Height),
        vdCocoaGetWindowConfig().Width,
        vdCocoaGetWindowConfig().Height);

    if(vdCocoaGetWindowConfig().Borderless)        
        self = [super initWithContentRect:contentRect styleMask:NSBorderlessWindowMask backing:NSBackingStoreBuffered defer:NO];
    else
        self = [super initWithContentRect:contentRect styleMask:NSTitledWindowMask  backing:NSBackingStoreBuffered defer:NO];

    if (self != nil)
    {
        // Start with no transparency for all drawing into the window
        [self setAlphaValue:1.0];
        // Turn off opacity so that the parts of the window that are not drawn into are transparent.
        [self setOpaque:NO];
    }
    
    [self setAcceptsMouseMovedEvents:YES];
    
    return self;
}

- (BOOL)acceptsMouseMovedEvents { return YES; }

- (BOOL)canBecomeKeyWindow { return YES; }

// mouseMoved doesn't fire on overlay windows.
// There is some hackery you can use:
// http://www.cocoabuilder.com/archive/cocoa/211715-getting-mouse-moved-events-on-overlay-windows.html

- (void) mouseMoved: (NSEvent*) theEvent
{
    NSPoint curPoint = [theEvent locationInWindow];
    NSRect windowFrame = [self frame];

    vdCocoaMotionCallback(curPoint.x, windowFrame.size.height - curPoint.y);
}

- (void) mouseDragged: (NSEvent*) theEvent
{
    NSPoint curPoint = [theEvent locationInWindow];
    NSRect windowFrame = [self frame];

    vdCocoaMotionCallback(curPoint.x, windowFrame.size.height - curPoint.y);
}

/*
 Start tracking a potential drag operation here when the user first clicks the mouse, to establish the initial location.
 */
- (void)otherMouseDown:(NSEvent *)theEvent {    
    // Get the mouse location in window coordinates.
    self.initialLocation = [theEvent locationInWindow];
}

- (void) mouseUp: (NSEvent*) theEvent
{
    NSRect frameRect = [self frame];
    NSPoint curPoint = [theEvent locationInWindow];

    int button = 1;
    int state = 0;
    int mx = curPoint.x;
    int my = frameRect.size.height - curPoint.y;

    vdCocoaMouseCallback(button, state, mx, my);
}

- (void) mouseDown: (NSEvent*) theEvent
{
    NSRect frameRect = [self frame];
    NSPoint curPoint = [theEvent locationInWindow];

    int button = 1;
    int state = 1;
    int mx = curPoint.x;
    int my = frameRect.size.height - curPoint.y;

    if (theEvent.clickCount == 2)
    {
        vdCocoaMouseCallback(button, state, mx, my);
    }
    else
    {
        vdCocoaMouseCallback(button, state, mx, my);
    }
}

- (void)otherMouseDragged:(NSEvent *)theEvent 
{
    //
    // If the window has no title bar, then emulate one so the user can change the window position
    //
    NSRect screenVisibleFrame = [[NSScreen mainScreen] visibleFrame];
    NSRect windowFrame = [self frame];
    NSPoint newOrigin = windowFrame.origin;

    // Get the mouse location in window coordinates.
    NSPoint currentLocation = [theEvent locationInWindow];

    // Update the origin with the difference between the new mouse location and the old mouse location.
    newOrigin.x += (currentLocation.x - initialLocation.x);
    newOrigin.y += (currentLocation.y - initialLocation.y);

    // Don't let window get dragged up under the menu bar
    if ((newOrigin.y + windowFrame.size.height) > (screenVisibleFrame.origin.y + screenVisibleFrame.size.height)) 
    {
        newOrigin.y = screenVisibleFrame.origin.y + (screenVisibleFrame.size.height - windowFrame.size.height);
    }
    
    // Move the window to the new location
    [self setFrameOrigin:newOrigin];
}

- (void) keyDown:(NSEvent *)theEvent
{
    NSString *characters;
    unsigned int characterIndex, characterCount;

    NSRect frameRect = [self frame];
    NSPoint curPoint = [theEvent locationInWindow];

    int mx = curPoint.x;
    int my = frameRect.size.height - curPoint.y;
    
    characters = [theEvent charactersIgnoringModifiers];
    characterCount = [characters length];

    for (characterIndex = 0; characterIndex < characterCount; characterIndex++)
    {
        unichar keyChar = [characters characterAtIndex:characterIndex];
        switch (keyChar)
        {
            case 27:
            case 'q':
                [[NSApplication sharedApplication] terminate:self];
                break;
            default:
                vdCocoaKeyboardCallback((unsigned char)(keyChar), mx, my);
                break;
        }
        [self onKey:keyChar downEvent:YES];
    }
}

- (void) keyUp:(NSEvent *)theEvent
{
    NSString *characters;
    unsigned int characterIndex, characterCount;

    NSRect frameRect = [self frame];
    NSPoint curPoint = [theEvent locationInWindow];

    int mx = curPoint.x;
    int my = frameRect.size.height - curPoint.y;
    
    characters = [theEvent charactersIgnoringModifiers];
    characterCount = [characters length];

    for (characterIndex = 0; characterIndex < characterCount; characterIndex++)
    {
        unichar keyChar = [characters characterAtIndex:characterIndex];
        switch (keyChar)
        {
            default:
                vdCocoaKeyboardUpCallback((unsigned char)(keyChar), mx, my);
                break;
        }
        [self onKey:keyChar downEvent:YES];
    }
}

@end

