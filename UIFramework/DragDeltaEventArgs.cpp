#include "DragDeltaEventArgs.h"

CDragDeltaEventArgs::CDragDeltaEventArgs() : m_HorizontalDelta(0),
                                             m_VerticalDelta(0)
{
}

CDragDeltaEventArgs::~CDragDeltaEventArgs()
{
}

HRESULT CDragDeltaEventArgs::Initialize(CRoutedEvent* pEvent, FLOAT HorizontalDelta, FLOAT VerticalDelta)
{
    HRESULT hr = S_OK;

    IFC(CRoutedEventArgs::Initialize(pEvent));

    m_HorizontalDelta = HorizontalDelta;
    m_VerticalDelta = VerticalDelta;

Cleanup:
    return hr;
}

FLOAT CDragDeltaEventArgs::GetHorizontalDelta()
{
    return m_HorizontalDelta;
}

FLOAT CDragDeltaEventArgs::GetVerticalDelta()
{
    return m_VerticalDelta;
}