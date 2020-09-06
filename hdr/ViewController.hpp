#import <UIKit/UIKit.h>
#import <Roole/Fwd.hpp>


@interface RooleViewController : UIViewController

- (void)trackTextInput:(Roole::Input&)input;

@property (nonatomic, assign) void *roleWindow;

@end
