#import "Controller.h"

@implementation Controller

- (IBAction)changeTransparency:(id)sender {	
    // Set the window's alpha value. This will cause the views in the window to redraw.
    [self.window setAlphaValue:[sender floatValue]];
}

@end
