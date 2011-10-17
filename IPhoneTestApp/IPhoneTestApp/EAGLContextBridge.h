#pragma once

#include "OpenGLES20Context.h"
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGL.h>

class EAGLContextBridge : public COpenGLES20Context
{
    public:
        virtual int AddRef(
            );
    
        virtual int Release(
            );
    
        virtual EAGLContext* GetContext() = 0;
    
        virtual bool Present(unsigned int RenderBuffer) = 0;
    
    protected:
        EAGLContextBridge(
            );
    
        virtual ~EAGLContextBridge(
            );
    
        int m_Ref;
};