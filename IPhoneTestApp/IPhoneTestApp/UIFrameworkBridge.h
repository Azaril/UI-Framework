#pragma once

class CDynamicClassResolver;
class CTypeConverter;
class CProviders;
class CParser;

#include "EAGLContextBridge.h"

class UIFrameworkBridge
{
public:
    UIFrameworkBridge(
    );
    
    virtual ~UIFrameworkBridge(
    );
    
    bool Initialize(
        EAGLContextBridge* pContext
        );
    
    CDynamicClassResolver* m_pClassResolver;
    CTypeConverter* m_pTypeConverter;
    CProviders* m_pProviders;
    CParser* m_pParser;
};