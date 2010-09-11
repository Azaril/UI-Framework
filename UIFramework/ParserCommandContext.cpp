#include "ParserCommandContext.h"

CParserCommandContext::CParserCommandContext(CProviders* pProviders, IParserCallback* pCallback) : m_Providers(pProviders),
                                                                                                   m_Callback(pCallback)
{
    AddRefObject(m_Providers);
}

CParserCommandContext::~CParserCommandContext()
{
    for(std::vector< CObjectWithType* >::iterator It = m_ObjectStack.begin(); It != m_ObjectStack.end(); ++It)
    {
        (*It)->Release();
    }

    ReleaseObject(m_Providers);
}

CProviders* CParserCommandContext::GetProviders()
{
    return m_Providers;
}

HRESULT CParserCommandContext::GetObject(CObjectWithType** ppObject)
{
    HRESULT hr = S_OK;
    CObjectWithType* pObject = NULL;

    IFCPTR(ppObject);

    IFCEXPECT(!m_ObjectStack.empty());

    pObject = m_ObjectStack.back();
    AddRefObject(pObject);

    *ppObject = pObject;
    pObject = NULL;

Cleanup:
    return hr;
}

HRESULT CParserCommandContext::PushObject(CObjectWithType* pObject)
{
    HRESULT hr = S_OK;

    IFCPTR(pObject);

    m_ObjectStack.push_back(pObject);
    AddRefObject(pObject);

    if(m_Callback)
    {
        IFC(m_Callback->OnPushObject(pObject));
    }

Cleanup:
    return hr;
}

HRESULT CParserCommandContext::PopObject()
{
    HRESULT hr = S_OK;
    CObjectWithType* pObject = NULL;

    IFCEXPECT(!m_ObjectStack.empty());

    pObject = m_ObjectStack.back();

    if(m_Callback)
    {
        IFC(m_Callback->OnPopObject(pObject));
    }

    m_ObjectStack.pop_back();

    ReleaseObject(pObject);

Cleanup:
    return hr;
}