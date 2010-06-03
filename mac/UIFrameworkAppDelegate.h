//
//  UIFrameworkAppDelegate.h
//  UIFramework
//
//  Created by William Archbell on 5/31/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface UIFrameworkAppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *window;
}

@property (assign) IBOutlet NSWindow *window;

@end
