#include "EAGLContextBridgeGLES20.h"

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

EAGLContext* EAGLContextBridgeGLES20::GetContext(
)
{
    return m_pContext;
}