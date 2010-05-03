#include "MouseInput.h"
#include "UIElement.h"

HRESULT CMouseInputHitTestFilter::Filter(CVisual* pVisual, HitTestFilterBehavior::Value* pFilterBehavior)
{
    HRESULT hr = S_OK;

    IFCPTR(pFilterBehavior);

    if(pVisual->IsTypeOf(TypeIndex::UIElement))
    {
        *pFilterBehavior = HitTestFilterBehavior::Continue;
    }
    else
    {
        *pFilterBehavior = HitTestFilterBehavior::ContinueSkipSelf;
    }

Cleanup:
    return hr;
}

CMouseButtonHitTestCallback::CMouseButtonHitTestCallback(Point2F MouseLocation, MouseButton::Value Button, MouseButtonState::Value State) : m_Button(Button),
                                                                                                                                            m_ButtonState(State),
                                                                                                                                            m_Location(MouseLocation)
{
}

HRESULT CMouseButtonHitTestCallback::ItemHit(CVisual* pVisual, HitTestResultBehavior::Value* pResultBehavior)
{
    HRESULT hr = S_OK;
    CMouseButtonEventArgs* pEventArgs = NULL;

    IFCPTR(pVisual);
    IFCPTR(pResultBehavior);

    if(pVisual->IsTypeOf(TypeIndex::UIElement))
    {
        CUIElement* pElement = (CUIElement*)pVisual;

        IFC(CMouseButtonEventArgs::Create(&CUIElement::MouseButtonEvent, m_Location, m_Button, m_ButtonState, &pEventArgs));

        IFC(pElement->RaiseEvent(pEventArgs));

        *pResultBehavior = HitTestResultBehavior::Stop;
    }
    else
    {
        *pResultBehavior = HitTestResultBehavior::Continue;
    }

Cleanup:
    ReleaseObject(pEventArgs);

    return hr;
}




CMouseMoveHitTestCallback::CMouseMoveHitTestCallback(Point2F MouseLocation) : m_Location(MouseLocation)
{
}

HRESULT CMouseMoveHitTestCallback::ItemHit(CVisual* pVisual, HitTestResultBehavior::Value* pResultBehavior)
{
    HRESULT hr = S_OK;
    CMouseEventArgs* pEventArgs = NULL;

    IFCPTR(pVisual);
    IFCPTR(pResultBehavior);

    if(pVisual->IsTypeOf(TypeIndex::UIElement))
    {
        CUIElement* pElement = (CUIElement*)pVisual;

        IFC(CMouseEventArgs::Create(&CUIElement::MouseMoveEvent, m_Location, &pEventArgs));

        IFC(pElement->RaiseEvent(pEventArgs));

        *pResultBehavior = HitTestResultBehavior::Stop;
    }
    else
    {
        *pResultBehavior = HitTestResultBehavior::Continue;
    }

Cleanup:
    ReleaseObject(pEventArgs);

    return hr;
}