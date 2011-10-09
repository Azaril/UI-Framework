//
//  IPhoneTestAppViewController.m
//  IPhoneTestApp
//
//  Created by William Archbell on 9/9/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>

#import "IPhoneTestAppViewController.h"
#import "EAGLView.h"
#include "UIFrameworkBridge.h"
#include "EAGLContextBridgeGLES20.h"
#include "EAGLLayerStorageAllocator.h"

@interface IPhoneTestAppViewController ()
@property (nonatomic, assign) CADisplayLink *displayLink;
@property UIFrameworkBridge* frameworkBridge;
@property EAGLContextBridgeGLES20* contextBridge;
@property EAGLLayerStorageAllocator* storageAllocator;
@end

@implementation IPhoneTestAppViewController

@synthesize animating;
@synthesize displayLink;
@synthesize frameworkBridge;
@synthesize contextBridge;
@synthesize storageAllocator;

const wchar_t g_MarkupContent[] =
L"<Canvas>"
L"<Border Height=\"100\" Width=\"100\" Canvas.Left=\"50\" Canvas.Top=\"150\" BorderThickness=\"5\" BorderBrush=\"Red\"/>"
L"<Border Height=\"100\" Width=\"100\" Canvas.Left=\"100\" Canvas.Top=\"170\" BorderThickness=\"5\" BorderBrush=\"Green\"/>"
L"<Border Height=\"100\" Width=\"100\" Canvas.Left=\"150\" Canvas.Top=\"190\" BorderThickness=\"5\" BorderBrush=\"Blue\"/>"
L"<Border Name=\"BlueBorder\" Height=\"100\" Width=\"100\" Canvas.Left=\"100\" Canvas.Top=\"100\">"
L"<Border.Background>"
//L"<SolidColorBrush Color=\"Red\"/>"
L"<LinearGradientBrush StartPoint=\"0,0\" EndPoint=\"1,1\">"
L"<GradientStop Color=\"Red\" Offset=\"0\" />"
L"<GradientStop Color=\"Blue\" Offset=\"0.25\" />"
L"<GradientStop Color=\"Green\" Offset=\"0.75\" />"
L"<GradientStop Color=\"Yellow\" Offset=\"1\" />"
L"</LinearGradientBrush>"
L"</Border.Background>"
L"<Border.Style><Style><Style.Triggers><EventTrigger RoutedEvent=\"UIElement.Loaded\">"
L"<BeginStoryboard>"
L"<Storyboard>"
//L"<FloatAnimation Storyboard.TargetName=\"BlueBorder\" Storyboard.TargetProperty=\"Canvas.Left\" Duration=\"5\" From=\"0\" To=\"200\" />"
L"</Storyboard>"
L"</BeginStoryboard>"
L"</EventTrigger></Style.Triggers></Style></Border.Style>"
L"</Border>"
L"</Canvas>";

- (void)awakeFromNib
{
    contextBridge = new EAGLContextBridgeGLES20();
    storageAllocator = new EAGLLayerStorageAllocator(contextBridge, (CAEAGLLayer*)self.view.layer);
    
    animating = FALSE;
    animationFrameInterval = 1;
    self.displayLink = nil;
    
    frameworkBridge = new UIFrameworkBridge();
    frameworkBridge->Initialize(contextBridge, storageAllocator);
    
    frameworkBridge->LoadContent(g_MarkupContent);
}

- (void)dealloc
{
    delete contextBridge;
    delete storageAllocator;
    
    // Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
    
    [context release];
    
    [super dealloc];
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

- (void)viewWillAppear:(BOOL)animated
{
    [self startAnimation];
    
    [super viewWillAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [self stopAnimation];
    
    [super viewWillDisappear:animated];
}

- (void)viewDidUnload
{
	[super viewDidUnload];
	
    if (program) {
        glDeleteProgram(program);
        program = 0;
    }

    // Tear down context.
//    if ([EAGLContext currentContext] == context)
//        [EAGLContext setCurrentContext:nil];
//	self.context = nil;	
}

- (NSInteger)animationFrameInterval
{
    return animationFrameInterval;
}

- (void)setAnimationFrameInterval:(NSInteger)frameInterval
{
    /*
	 Frame interval defines how many display frames must pass between each time the display link fires.
	 The display link will only fire 30 times a second when the frame internal is two on a display that refreshes 60 times a second. The default frame interval setting of one will fire 60 times a second when the display refreshes at 60 times a second. A frame interval setting of less than one results in undefined behavior.
	 */
    if (frameInterval >= 1) {
        animationFrameInterval = frameInterval;
        
        if (animating) {
            [self stopAnimation];
            [self startAnimation];
        }
    }
}

- (void)startAnimation
{
    if (!animating) {
        CADisplayLink *aDisplayLink = [[UIScreen mainScreen] displayLinkWithTarget:self selector:@selector(drawFrame)];
        [aDisplayLink setFrameInterval:animationFrameInterval];
        [aDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        self.displayLink = aDisplayLink;
        
        animating = TRUE;
    }
}

- (void)stopAnimation
{
    if (animating) {
        [self.displayLink invalidate];
        self.displayLink = nil;
        animating = FALSE;
    }
}

- (void)drawFrame
{
    frameworkBridge->Render();

    contextBridge->Present(frameworkBridge->GetRenderBuffer());
}

@end
