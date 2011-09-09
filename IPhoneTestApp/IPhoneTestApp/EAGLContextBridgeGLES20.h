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
    
    EAGLContext* GetContext(
        );
    
protected:
    EAGLContext* m_pContext;
};