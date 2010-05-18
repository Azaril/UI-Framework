#include "MouseController.h"

CMouseController::CMouseController() : m_RootElement(NULL),
                                       m_MouseOverElement(NULL),
                                       m_CaptureElement(NULL)
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

    IFCPTR(pRootElement);

    m_RootElement = pRootElement;
    AddRefObject(m_RootElement);

Cleanup:
    return hr;
}

HRESULT CMouseController::InjectMouseButton(MouseButton::Value Button, MouseButtonState::Value State, Point2F Location)
{
    HRESULT hr = S_OK;

    IFC(InjectMouseMove(Location));

    {
        CMouseInputHitTestFilter Filter;
        CMouseButtonHitTestCallback Callback(this, Location, Button, State);

        IFC(HitTestTree(m_RootElement, Location, &Filter, &Callback));
    }

Cleanup:
    return hr;
}

HRESULT CMouseController::InjectMouseMove(Point2F Location)
{
    HRESULT hr = S_OK;

    CMouseInputHitTestFilter Filter;
    CMouseMoveHitTestCallback Callback(this, Location);

    IFC(HitTestTree(m_RootElement, Location, &Filter, &Callback));

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

HRESULT CMouseController::RaiseMouseMove(Point2F Location)
{
    HRESULT hr = S_OK;
    CMouseEventArgs* pEventArgs = NULL;

    //TODO: Raise event to element that has requested mouse capture.

    if(m_MouseOverElement)
    {
        IFC(CMouseEventArgs::Create(&CUIElement::MouseMoveEvent, Location, &pEventArgs));

        IFC(m_MouseOverElement->RaiseEvent(pEventArgs));
    }

Cleanup:
    ReleaseObject(pEventArgs);

    return hr;
}

HRESULT CMouseController::RaiseMouseButton(Point2F Location, MouseButton::Value Button, MouseButtonState::Value State)
{
    HRESULT hr = S_OK;
    CMouseButtonEventArgs* pEventArgs = NULL;

    //TODO: Raise event to element that has requested mouse capture.

    if(m_MouseOverElement)
    {
        IFC(CMouseButtonEventArgs::Create(&CUIElement::MouseButtonEvent, Location, Button, State, &pEventArgs));

        IFC(m_MouseOverElement->RaiseEvent(pEventArgs));
    }

Cleanup:
    ReleaseObject(pEventArgs);

    return hr;
}