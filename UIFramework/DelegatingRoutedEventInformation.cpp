#include "DelegatingRoutedEventInformation.h"

CDelegatingRoutedEventInformation::CDelegatingRoutedEventInformation() : m_Root(NULL),
                                                                         m_Base(NULL)
{
}

CDelegatingRoutedEventInformation::~CDelegatingRoutedEventInformation()
{
    ReleaseObject(m_Root);
    ReleaseObject(m_Base);
}

HRESULT CDelegatingRoutedEventInformation::Initialize(CEventInformation* pRoot, CEventInformation* pBase)
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

HRESULT CDelegatingRoutedEventInformation::GetEvent(const WCHAR* pEventName, CRoutedEvent** ppEvent)
{
    HRESULT hr = S_OK;

    IFCPTR(pEventName);
    IFCPTR(ppEvent);

    if(FAILED(m_Root->GetEvent(pEventName, ppEvent)))
    {
        IFC(m_Base->GetEvent(pEventName, ppEvent));
    }

Cleanup:
    return hr;
}