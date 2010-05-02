#include "RoutedEventArgs.h"

CRoutedEventArgs::CRoutedEventArgs() : m_RoutedEvent(NULL),
                                       m_Handled(FALSE)
{
}

CRoutedEventArgs::~CRoutedEventArgs()
{
    ReleaseObject(m_RoutedEvent);
}

HRESULT CRoutedEventArgs::Initialize(CRoutedEvent* pRoutedEvent)
{
    HRESULT hr = S_OK;

    IFCPTR(pRoutedEvent);

    m_RoutedEvent = pRoutedEvent;
    AddRefObject(m_RoutedEvent);

Cleanup:
    return hr;
}

CRoutedEvent* CRoutedEventArgs::GetRoutedEvent()
{
    return m_RoutedEvent;
}

BOOL CRoutedEventArgs::IsHandled()
{
    return m_Handled;
}

void CRoutedEventArgs::SetHandled(BOOL Handled)
{
    m_Handled = Handled;
}