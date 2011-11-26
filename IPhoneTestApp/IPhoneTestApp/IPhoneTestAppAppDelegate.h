#import <UIKit/UIKit.h>

@class IPhoneTestAppViewController;

@interface IPhoneTestAppAppDelegate : NSObject <UIApplicationDelegate>

@property (nonatomic, retain) IBOutlet UIWindow *window;

@property (nonatomic, retain) IBOutlet IPhoneTestAppViewController *viewController;

@end
