#include "EAGLContextBridge.h"

EAGLContextBridge::EAGLContextBridge(
    )
    : m_Ref(1)
{
}

EAGLContextBridge::~EAGLContextBridge(
    )
{
}

int
EAGLContextBridge::AddRef(
    )
{
    int ref = ++m_Ref;
    
    return ref;
}

int
EAGLContextBridge::Release(
    )
{
    int ref = --m_Ref;
    
    if (ref == 0)
    {
        delete this;
    }
    
    return ref;
}