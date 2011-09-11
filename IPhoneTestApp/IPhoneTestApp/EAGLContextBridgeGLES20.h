#pragma once

#import <OpenGLES/EAGL.h>

#include "EAGLContextBridge.h"

class EAGLContextBridgeGLES20 : public EAGLContextBridge
{
public:
    EAGLContextBridgeGLES20(
    );
    
    virtual ~EAGLContextBridgeGLES20(
    );
    
    virtual bool Apply(
        );
    
    virtual bool Present(
        unsigned int RenderBuffer
        );
    
    virtual EAGLContext* GetContext(
        );
    
protected:
    EAGLContext* m_pContext;
};