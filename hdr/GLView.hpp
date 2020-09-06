#import <UIKit/UIKit.h>
#import <Roole/Fwd.hpp>

@interface RooleGLView : UIView <UIKeyInput>

- (instancetype)initWithFrame:(CGRect)frame NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithCoder:(NSCoder*)aDecoder NS_UNAVAILABLE;

- (void)redrawGL:(void (^)())code;

// This property must be set in order for TextInput handling to work.
@property (nonatomic, assign) Roole::Input* input;

@end
