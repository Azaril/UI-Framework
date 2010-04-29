#include "DelegatingPropertyInformation.h"

CDelegatingPropertyInformation::CDelegatingPropertyInformation() : m_Root(NULL),
                                                                   m_Base(NULL)
{
}

CDelegatingPropertyInformation::~CDelegatingPropertyInformation()
{
    ReleaseObject(m_Root);
    ReleaseObject(m_Base);
}

HRESULT CDelegatingPropertyInformation::Initialize(CPropertyInformation* pRoot, CPropertyInformation* pBase)
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

HRESULT CDelegatingPropertyInformation::GetProperty(const WCHAR* pPropertyName, CProperty** ppProperty)
{
    HRESULT hr = S_OK;

    IFCPTR(pPropertyName);
    IFCPTR(ppProperty);

    if(FAILED(m_Root->GetProperty(pPropertyName, ppProperty)))
    {
        IFC(m_Base->GetProperty(pPropertyName, ppProperty));
    }

Cleanup:
    return hr;
}

HRESULT CDelegatingPropertyInformation::GetContentProperty(CProperty** ppProperty)
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