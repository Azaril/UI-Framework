#include "EAGLContextBridgeGLES20.h"

#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#import <QuartzCore/QuartzCore.h>

EAGLContextBridgeGLES20::EAGLContextBridgeGLES20(
    )
    : m_pContext(NULL)
{
    m_pContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
}

EAGLContextBridgeGLES20::~EAGLContextBridgeGLES20(
)
{
    if (m_pContext != NULL)
    {
        // Tear down context.
        if ([EAGLContext currentContext] == m_pContext)
            [EAGLContext setCurrentContext:nil];
        
        [m_pContext release];
    }
}

bool 
EAGLContextBridgeGLES20::Apply(
    )
{
    if ([EAGLContext currentContext] != m_pContext)
    {
        [EAGLContext setCurrentContext:m_pContext];
    }
    
    return true;
}

bool
EAGLContextBridgeGLES20::Present(
    unsigned int RenderBuffer
    )
{
    [m_pContext presentRenderbuffer:RenderBuffer];
    
    return true;
}

EAGLContext*
EAGLContextBridgeGLES20::GetContext(
    )
{
    return m_pContext;
}