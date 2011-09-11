#pragma once

#include "OpenGLES20RenderBufferStorageAllocator.h"
#include "EAGLContextBridge.h"

class EAGLLayerStorageAllocator : public COpenGLES20RenderBufferStorageAllocator
{
    public:
        EAGLLayerStorageAllocator(
            EAGLContextBridge* pContext,
            CAEAGLLayer* pLayer
            );
        
        virtual ~EAGLLayerStorageAllocator(
            );
    
        virtual bool AllocateStorage(
            GLuint RenderBuffer
            );
    
    EAGLContextBridge* m_pContext;
    CAEAGLLayer* m_pLayer;
};