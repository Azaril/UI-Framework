#include "MouseController.h"

CMouseController::CMouseController() : m_RootElement(NULL),
                                       m_MouseOverElement(NULL),
                                       m_CaptureElement(NULL),
                                       m_Position(0, 0)
{
}

CMouseController::~CMouseController()
{
    ReleaseObject(m_RootElement);
    ReleaseObject(m_MouseOverElement);
    ReleaseObject(m_CaptureElement);
}

HRESULT CMouseController::Initialize(CUIElement* pRootElement)
{
    HRESULT hr = S_OK;

    m_RootElement = pRootElement;
    AddRefObject(m_RootElement);

Cleanup:
    return hr;
}

HRESULT CMouseController::InjectMouseButton(MouseButton::Value Button, MouseButtonState::Value State, BOOL* pConsumed)
{
    HRESULT hr = S_OK;

    if(m_CaptureElement == NULL)
    {
        CMouseInputHitTestFilter Filter;
        CMouseButtonHitTestCallback Callback(this, m_Position, Button, State, pConsumed);

        IFC(HitTestTree(m_RootElement, m_Position, &Filter, &Callback));
    }
    else
    {
        IFC(RaiseMouseButton(m_Position, Button, State, pConsumed));
    }

Cleanup:
    return hr;
}

HRESULT CMouseController::InjectMouseMove(Point2F Location, BOOL* pConsumed)
{
    HRESULT hr = S_OK;

    if(m_CaptureElement == NULL)
    {
        CMouseInputHitTestFilter Filter;
        CMouseMoveHitTestCallback Callback(this, Location, pConsumed);

        IFC(HitTestTree(m_RootElement, Location, &Filter, &Callback));
    }
    else
    {
        IFC(RaiseMouseMove(Location, pConsumed));
    }

    m_Position = Location;

Cleanup:
    return hr;
}

HRESULT CMouseController::SetMouseOverElement(CUIElement* pElement, Point2F Location)
{
    HRESULT hr = S_OK;
    CMouseEventArgs* pMouseOutEventArgs = NULL;
    CMouseEventArgs* pMouseOverEventArgs = NULL;

    if(pElement != m_MouseOverElement)
    {
        if(m_MouseOverElement)
        {
            IFC(CMouseEventArgs::Create(&CUIElement::MouseLeaveEvent, Location, &pMouseOutEventArgs));

            IFC(m_MouseOverElement->RaiseEvent(pMouseOutEventArgs));

            ReleaseObject(m_MouseOverElement);
        }

        m_MouseOverElement = pElement;

        if(m_MouseOverElement)
        {
            AddRefObject(m_MouseOverElement);

            IFC(CMouseEventArgs::Create(&CUIElement::MouseEnterEvent, Location, &pMouseOverEventArgs));

            IFC(m_MouseOverElement->RaiseEvent(pMouseOverEventArgs));

            //TODO: Hook up to events that would affect hit testing (visibility etc.)
        }
    }

Cleanup:
    ReleaseObject(pMouseOutEventArgs);
    ReleaseObject(pMouseOverEventArgs);

    return hr;
}

HRESULT CMouseController::RaiseMouseMove(Point2F Location, BOOL* pHandled)
{
    HRESULT hr = S_OK;
    CMouseEventArgs* pEventArgs = NULL;
    BOOL Handled = FALSE;
    CUIElement* pTargetElement = NULL;

    pTargetElement = (m_CaptureElement != NULL) ? m_CaptureElement : m_MouseOverElement;

    if(pTargetElement != NULL)
    {
        IFC(CMouseEventArgs::Create(&CUIElement::MouseMoveEvent, Location, &pEventArgs));

        IFC(pTargetElement->RaiseEvent(pEventArgs));

        Handled = pEventArgs->IsHandled();
    }

    if(pHandled)
    {
        *pHandled = Handled;
    }

Cleanup:
    ReleaseObject(pEventArgs);

    return hr;
}

HRESULT CMouseController::RaiseMouseButton(Point2F Location, MouseButton::Value Button, MouseButtonState::Value State, BOOL* pHandled)
{
    HRESULT hr = S_OK;
    CMouseButtonEventArgs* pEventArgs = NULL;
    BOOL Handled = FALSE;
    CUIElement* pTargetElement = NULL;

    pTargetElement = (m_CaptureElement != NULL) ? m_CaptureElement : m_MouseOverElement;

    if(pTargetElement != NULL)
    {
        IFC(CMouseButtonEventArgs::Create(&CUIElement::MouseButtonEvent, Location, Button, State, &pEventArgs));

        IFC(pTargetElement->RaiseEvent(pEventArgs));

        Handled = pEventArgs->IsHandled();
    }

    if(pHandled)
    {
        *pHandled = Handled;
    }

Cleanup:
    ReleaseObject(pEventArgs);

    return hr;
}

HRESULT CMouseController::SetCapture(CUIElement* pElement)
{
    HRESULT hr = S_OK;

    if(m_CaptureElement == NULL)
    {
        m_CaptureElement = pElement;
        AddRefObject(m_CaptureElement);
    }

Cleanup:
    return hr;
}

HRESULT CMouseController::ReleaseCapture(CUIElement* pElement)
{
    HRESULT hr = S_OK;

    if(m_CaptureElement == pElement)
    {
        ReleaseObject(m_CaptureElement);
    }

Cleanup:
    return hr;
}