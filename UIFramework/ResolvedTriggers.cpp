#include "ResolvedTriggers.h"

CResolvedTriggers::CResolvedTriggers() : m_Owner(NULL),
                                         m_Providers(NULL),
                                         m_Callback(NULL)
{
}

CResolvedTriggers::~CResolvedTriggers()
{
    for(std::vector< CResolvedTrigger* >::iterator It = m_Triggers.begin(); It != m_Triggers.end(); ++It)
    {
        (*It)->Release();
    }

    m_Triggers.clear();

    ReleaseObject(m_Providers);
}

HRESULT CResolvedTriggers::Initialize(CUIElement* pObject, CProviders* pProviders, IStyleCallback* pCallback)
{
    HRESULT hr = S_OK;

    IFCPTR(pObject);
    IFCPTR(pProviders);
    IFCPTR(pCallback);

    //NOTE: Don't add ref the owner or a circular dependency will be created.
    m_Owner = pObject;    

    m_Providers = pProviders;
    AddRefObject(m_Providers);

    m_Callback = pCallback;

Cleanup:
    return hr;
}

HRESULT CResolvedTriggers::AddTrigger(CTrigger* pTrigger)
{
    HRESULT hr = S_OK;
    CResolvedTrigger* pResolvedTrigger = NULL;

    IFCPTR(pTrigger);

    if(SUCCEEDED(pTrigger->ResolveTrigger(m_Owner, m_Providers, m_Callback, &pResolvedTrigger)))
    {
        m_Triggers.push_back(pResolvedTrigger);

        pResolvedTrigger = NULL;
    }

Cleanup:
    ReleaseObject(pResolvedTrigger);

    return hr;
}