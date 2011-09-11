#pragma once

#include "OpenGLES20Context.h"
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGL.h>

class EAGLContextBridge : public COpenGLES20Context
{
    public:
        virtual EAGLContext* GetContext() = 0;
    
        virtual bool Present(unsigned int RenderBuffer) = 0;
};