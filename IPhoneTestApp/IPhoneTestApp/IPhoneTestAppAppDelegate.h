//
//  IPhoneTestAppAppDelegate.h
//  IPhoneTestApp
//
//  Created by William Archbell on 9/9/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class IPhoneTestAppViewController;

@interface IPhoneTestAppAppDelegate : NSObject <UIApplicationDelegate>

@property (nonatomic, retain) IBOutlet UIWindow *window;

@property (nonatomic, retain) IBOutlet IPhoneTestAppViewController *viewController;

@end
