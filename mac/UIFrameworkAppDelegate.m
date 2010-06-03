//
//  UIFrameworkAppDelegate.m
//  UIFramework
//
//  Created by William Archbell on 5/31/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "UIFrameworkAppDelegate.h"
#include <agl/agl.h>

@implementation UIFrameworkAppDelegate

@synthesize window;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification 
{
//    OSStatus err = noErr;
//    Rect rectPort;
    GLint attributes[] =  { AGL_RGBA, AGL_DOUBLEBUFFER, AGL_DEPTH_SIZE, 24, AGL_NONE };
    AGLContext  myAGLContext = NULL;
    AGLPixelFormat myAGLPixelFormat;
    
    myAGLPixelFormat = aglChoosePixelFormat (NULL, 0, attributes);
    //err = MyAGLReportError ();
    if (myAGLPixelFormat) {
        myAGLContext = aglCreateContext (myAGLPixelFormat, NULL);
        //err = MyAGLReportError ();
    }
    if (! aglSetWindowRef (myAGLContext, [window windowRef])) { }
        //err = MyAGLReportError ();
    if (!aglSetCurrentContext (myAGLContext)) { }
        //err = MyAGLReportError ();
//    return err;    
	// Insert code here to initialize your application 
}

@end
