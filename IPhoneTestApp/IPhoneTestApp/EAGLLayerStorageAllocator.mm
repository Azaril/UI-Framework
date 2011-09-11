#include "EAGLLayerStorageAllocator.h"
#include "EAGLContextBridge.h"

EAGLLayerStorageAllocator::EAGLLayerStorageAllocator(
     EAGLContextBridge* pContext,
     CAEAGLLayer* pLayer
     )
    : m_pContext(pContext)
    , m_pLayer(pLayer)
{    
}

EAGLLayerStorageAllocator::~EAGLLayerStorageAllocator(
    )
{
}

bool
EAGLLayerStorageAllocator::AllocateStorage(
   GLuint RenderBuffer
   )
{
    [m_pContext->GetContext() renderbufferStorage:RenderBuffer fromDrawable:m_pLayer];
    
    return true;
}