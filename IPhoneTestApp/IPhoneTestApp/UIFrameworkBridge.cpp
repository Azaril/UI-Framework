#include "UIFrameworkBridge.h"
#include "BasicTypeConverter.h"
#include "DynamicClassResolver.h"
#include "Parser.h"

UIFrameworkBridge::UIFrameworkBridge(
    )
    : m_pClassResolver(NULL)
    , m_pTypeConverter(NULL)
    , m_pProviders(NULL)
    , m_pParser(NULL)
{
}

UIFrameworkBridge::~UIFrameworkBridge(
    )
{
    ReleaseObject(m_pClassResolver);
    ReleaseObject(m_pTypeConverter);
    ReleaseObject(m_pProviders);
    ReleaseObject(m_pParser);
}

bool
UIFrameworkBridge::Initialize(
    EAGLContextBridge* pContextBridge
    )
{
    HRESULT hr = S_OK;
    
    IFC(CDynamicClassResolver::Create(&m_pClassResolver));
    
    IFC(CreateBasicTypeConverter(&m_pTypeConverter));
    
    IFC(CProviders::Create(m_pClassResolver, m_pTypeConverter, &m_pProviders));
    
    IFC(CParser::Create(m_pProviders, &m_pParser));
    
Cleanup:
    return SUCCEEDED(hr);
}