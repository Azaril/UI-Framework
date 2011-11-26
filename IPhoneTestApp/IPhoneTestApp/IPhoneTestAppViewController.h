//
//  IPhoneTestAppViewController.h
//  IPhoneTestApp
//
//  Created by William Archbell on 9/9/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

#import <OpenGLES/EAGL.h>

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#include "BasicTypeConverter.h"
#include "DynamicClassResolver.h"
#include "Parser.h"
#include "UIHost.h"
#include "FileResourceProvider.h"
#include "BundleFileResourceProvider.h"
#include "CompositeResourceProvider.h"
#include "OpenGLES20GraphicsDevice.h"
#include "OpenGLES20RenderTarget.h"
#include "MouseController.h"

@interface IPhoneTestAppViewController : UIViewController {
    EAGLContext *context;
    
    BOOL animating;
    NSInteger animationFrameInterval;
    CADisplayLink *displayLink;
    
    CDynamicClassResolver* m_pClassResolver;
    CTypeConverter* m_pTypeConverter;
    CProviders* m_pProviders;
    CParser* m_pParser;
    CUIHost* m_pUIHost;
    CFileResourceProvider* m_pFileResourceProvider;
    CBundleFileResourceProvider* m_pBundleResourceProvider;
    CCompositeResourceProvider* m_pCompositeResourceProvider;
    COpenGLES20GraphicsDevice* m_pGraphicsDevice;
    COpenGLES20RenderTarget* m_pRenderTarget;
    CMouseController* m_pMouseController;      
}

@property (readonly, nonatomic, getter=isAnimating) BOOL animating;
@property (nonatomic) NSInteger animationFrameInterval;

- (id)init;
- (void)startAnimation;
- (void)stopAnimation;

- (HRESULT)LoadContent:(IReadStream*)pContentStream;

@end
