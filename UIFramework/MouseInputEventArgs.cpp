#include "MouseInputEventArgs.h"

CMouseEventArgs::CMouseEventArgs()
{
}

CMouseEventArgs::~CMouseEventArgs()
{
}

HRESULT CMouseEventArgs::Initialize(CRoutedEvent* pRoutedEvent, Point2F MouseLocation)
{
    HRESULT hr = S_OK;

    IFCPTR(pRoutedEvent);

    IFC(CInputEventArgs::Initialize(pRoutedEvent));

    m_Location = MouseLocation;

Cleanup:
    return hr;
}

HRESULT CMouseEventArgs::Initialize(CRoutedEvent* pRoutedEvent, CMouseEventArgs* pSourceArgs)
{
    HRESULT hr = S_OK;

    IFCPTR(pRoutedEvent);
    IFCPTR(pSourceArgs);

    IFC(CInputEventArgs::Initialize(pRoutedEvent));

    m_Location = pSourceArgs->GetLocation();

Cleanup:
    return hr;
}

Point2F CMouseEventArgs::GetLocation()
{
    return m_Location;
}


CMouseButtonEventArgs::CMouseButtonEventArgs()
{
}

CMouseButtonEventArgs::~CMouseButtonEventArgs()
{
}

HRESULT CMouseButtonEventArgs::Initialize(CRoutedEvent* pRoutedEvent, Point2F MouseLocation, MouseButton::Value Button, MouseButtonState::Value ButtonState)
{
    HRESULT hr = S_OK;

    IFCPTR(pRoutedEvent);

    IFC(CMouseEventArgs::Initialize(pRoutedEvent, MouseLocation));

    m_Button = Button;
    m_ButtonState = ButtonState;

Cleanup:
    return hr;
}

HRESULT CMouseButtonEventArgs::Initialize(CRoutedEvent* pRoutedEvent, CMouseButtonEventArgs* pSourceArgs)
{
    HRESULT hr = S_OK;

    IFCPTR(pRoutedEvent);
    IFCPTR(pSourceArgs);

    IFC(CMouseEventArgs::Initialize(pRoutedEvent, pSourceArgs));

    m_Button = pSourceArgs->GetChangedButton();
    m_ButtonState = pSourceArgs->GetButtonState();

Cleanup:
    return hr;
}

MouseButton::Value CMouseButtonEventArgs::GetChangedButton()
{
    return m_Button;
}

MouseButtonState::Value CMouseButtonEventArgs::GetButtonState()
{
    return m_ButtonState;
}