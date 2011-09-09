#include "ResolvedSetters.h"

CResolvedSetters::CResolvedSetters(
    ) 
    : m_Owner(NULL)
    , m_Providers(NULL)
    , m_Callback(NULL)
{
}

CResolvedSetters::~CResolvedSetters(
    )
{
    for(std::vector< CResolvedSetter* >::iterator It = m_Setters.begin(); It != m_Setters.end(); ++It)
    {
        (*It)->Release();
    }

    m_Setters.clear();

    ReleaseObject(m_Providers);
}

__checkReturn HRESULT 
CResolvedSetters::Initialize(
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
CResolvedSetters::AddSetter(
    __in CSetter* pSetter
    )
{
    HRESULT hr = S_OK;
    CResolvedSetter* pResolvedSetter = NULL;

    IFCPTR(pSetter);

    if(SUCCEEDED(pSetter->ResolveSetter(m_Owner, m_Callback, &pResolvedSetter)))
    {
        m_Setters.push_back(pResolvedSetter);

        pResolvedSetter = NULL;
    }

Cleanup:
    ReleaseObject(pResolvedSetter);

    return hr;
}

__checkReturn HRESULT
CResolvedSetters::Apply(
    )
{
    HRESULT hr = S_OK;

    for(std::vector< CResolvedSetter* >::iterator It = m_Setters.begin(); It != m_Setters.end(); ++It)
    {
        IFC((*It)->Apply());
    }

Cleanup:
    return hr;
}