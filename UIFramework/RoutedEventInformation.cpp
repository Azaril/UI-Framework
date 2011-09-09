#include "RoutedEventInformation.h"
#include "RoutedEvent.h"

CRoutedEventInformation::CRoutedEventInformation(
    )
{
}

CRoutedEventInformation::~CRoutedEventInformation(
    )
{
    for(std::vector< CRoutedEvent* >::iterator It = m_Events.begin(); It != m_Events.end(); ++It)
    {
        (*It)->Release();
    }

    m_Events.clear();
}

__checkReturn HRESULT 
CRoutedEventInformation::Initialize(
    __in_ecount(EventCount) CRoutedEvent** ppEvents, 
    UINT32 EventCount
    )
{
    HRESULT hr = S_OK;

    if(EventCount > 0)
    {
        IFCPTR(ppEvents);
    }

    for(UINT32 i = 0; i < EventCount; i++)
    {
        CRoutedEvent* pEvent = ppEvents[i];

        IFCPTR(pEvent);

        m_Events.push_back(pEvent);

        AddRefObject(pEvent);
    }

Cleanup:
    return hr;
}

__override __checkReturn HRESULT
CRoutedEventInformation::GetEvent(
    __in_z const WCHAR* pEventName,
    __deref_out_opt CRoutedEvent** ppEvent
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pEventName);
    IFCPTR(ppEvent);

    for(std::vector< CRoutedEvent* >::iterator It = m_Events.begin(); It != m_Events.end(); ++It)
    {
        CRoutedEvent* pEvent = (*It);

        if(wcscmp(pEventName, pEvent->GetName()) == 0)
        {
            *ppEvent = pEvent;
            AddRefObject(pEvent);

            goto Cleanup;
        }
    }

    hr = E_FAIL;

Cleanup:
    return hr;
}