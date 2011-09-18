#pragma once

#include <OpenGLES/ES2/gl.h>

class COpenGLES20RenderBufferStorageAllocator
{
    public:
        virtual bool AllocateStorage( 
            GLuint RenderBuffer
            ) = 0;
};