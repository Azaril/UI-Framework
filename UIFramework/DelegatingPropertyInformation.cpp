#include "DelegatingPropertyInformation.h"

CDelegatingPropertyInformation::CDelegatingPropertyInformation(
    ) 
    : m_Root(NULL)
    , m_Base(NULL)
{
}

CDelegatingPropertyInformation::~CDelegatingPropertyInformation(
    )
{
    ReleaseObject(m_Root);
    ReleaseObject(m_Base);
}

__checkReturn HRESULT 
CDelegatingPropertyInformation::Initialize(
    __in CPropertyInformation* pRoot,
    __in CPropertyInformation* pBase
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pRoot);
    IFCPTR(pBase);

    m_Root = pRoot;
    AddRefObject(m_Root);

    m_Base = pBase;
    AddRefObject(m_Base);

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CDelegatingPropertyInformation::GetProperty(
    __in_z const WCHAR* pPropertyName,
    __in CProperty** ppProperty
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pPropertyName);
    IFCPTR(ppProperty);

    if(FAILED(m_Root->GetProperty(pPropertyName, ppProperty)))
    {
        IFC_NOTRACE(m_Base->GetProperty(pPropertyName, ppProperty));
    }

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CDelegatingPropertyInformation::GetContentProperty(
    __deref_out_opt CProperty** ppProperty
    )
{
    HRESULT hr = S_OK;

    IFCPTR(ppProperty);

    if(FAILED(m_Root->GetContentProperty(ppProperty)))
    {
        IFC(m_Base->GetContentProperty(ppProperty));
    }

Cleanup:
    return hr;
}