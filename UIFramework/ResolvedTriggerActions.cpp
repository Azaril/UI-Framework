#include "ResolvedTriggerActions.h"

CResolvedTriggerActions::CResolvedTriggerActions(
    ) 
    : m_Owner(NULL)
    , m_Providers(NULL)
    , m_Callback(NULL)
{
}

CResolvedTriggerActions::~CResolvedTriggerActions(
    )
{
    for(std::vector< CResolvedTriggerAction* >::iterator It = m_Actions.begin(); It != m_Actions.end(); ++It)
    {
        (*It)->Release();
    }

    ReleaseObject(m_Providers);
}

__checkReturn HRESULT 
CResolvedTriggerActions::Initialize(
    __in CUIElement* pObject,
    __in CProviders* pProviders, 
    __in IStyleCallback* pCallback
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pObject);
    IFCPTR(pProviders);

    //NOTE: Don't add ref the owner or a circular dependency will be created.
    m_Owner = pObject;    

    m_Providers = pProviders;
    AddRefObject(m_Providers);

    m_Callback = pCallback;

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CResolvedTriggerActions::AddAction(
    __in CTriggerAction* pAction
    )
{
    HRESULT hr = S_OK;
    CResolvedTriggerAction* pResolvedAction = NULL;

    IFCPTR(pAction);

    if(SUCCEEDED(pAction->ResolveAction(m_Owner, m_Callback, &pResolvedAction)))
    {
        m_Actions.push_back(pResolvedAction);

        pResolvedAction = NULL;
    }

Cleanup:
    ReleaseObject(pResolvedAction);

    return hr;
}

__checkReturn HRESULT
CResolvedTriggerActions::Apply(
    )
{
    HRESULT hr = S_OK;

    for(std::vector< CResolvedTriggerAction* >::iterator It = m_Actions.begin(); It != m_Actions.end(); ++It)
    {
        IFC((*It)->Apply());
    }

Cleanup:
    return hr;
}