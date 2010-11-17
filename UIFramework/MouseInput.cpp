#include "MouseInput.h"
#include "UIElement.h"
#include "MouseController.h"

HRESULT CMouseInputHitTestFilter::Filter(CVisual* pVisual, HitTestFilterBehavior::Value* pFilterBehavior)
{
    HRESULT hr = S_OK;

    IFCPTR(pFilterBehavior);

    if(pVisual->IsTypeOf(TypeIndex::UIElement))
    {
        BOOL HitTestVisible = FALSE;
        CUIElement* pElement = (CUIElement*)pVisual;

        IFC(pElement->IsHitTestVisible(&HitTestVisible));

        *pFilterBehavior = HitTestVisible ? HitTestFilterBehavior::Continue : HitTestFilterBehavior::ContinueSkipSelfAndChildren;
    }
    else
    {
        *pFilterBehavior = HitTestFilterBehavior::ContinueSkipSelf;
    }

Cleanup:
    return hr;
}

CMouseButtonHitTestCallback::CMouseButtonHitTestCallback(CMouseController* pController, Point2F MouseLocation, MouseButton::Value Button, MouseButtonState::Value State, BOOL* pHandled) : m_Controller(pController),
                                                                                                                                                                                           m_Button(Button),
                                                                                                                                                                                           m_ButtonState(State),
                                                                                                                                                                                           m_Location(MouseLocation),
                                                                                                                                                                                           m_Handled(pHandled)
{
}

HRESULT CMouseButtonHitTestCallback::ItemHit(CVisual* pVisual, HitTestResultBehavior::Value* pResultBehavior)
{
    HRESULT hr = S_OK;

    IFCPTR(pVisual);
    IFCPTR(pResultBehavior);

    if(pVisual->IsTypeOf(TypeIndex::UIElement))
    {
        CUIElement* pElement = (CUIElement*)pVisual;

        IFC(m_Controller->RaiseMouseButton(m_Location, m_Button, m_ButtonState, m_Handled));

        *pResultBehavior = HitTestResultBehavior::Stop;
    }
    else
    {
        *pResultBehavior = HitTestResultBehavior::Continue;
    }

Cleanup:
    return hr;
}




CMouseMoveHitTestCallback::CMouseMoveHitTestCallback(CMouseController* pController, Point2F MouseLocation, BOOL* pHandled) : m_Controller(pController),
                                                                                                                             m_Location(MouseLocation),
                                                                                                                             m_Handled(pHandled)
{
}

HRESULT CMouseMoveHitTestCallback::ItemHit(CVisual* pVisual, HitTestResultBehavior::Value* pResultBehavior)
{
    HRESULT hr = S_OK;

    IFCPTR(pVisual);
    IFCPTR(pResultBehavior);

    if(pVisual->IsTypeOf(TypeIndex::UIElement))
    {
        CUIElement* pElement = (CUIElement*)pVisual;

        IFC(m_Controller->SetMouseOverElement(pElement, m_Location));

        IFC(m_Controller->RaiseMouseMove(m_Location, m_Handled));

        *pResultBehavior = HitTestResultBehavior::Stop;
    }
    else
    {
        *pResultBehavior = HitTestResultBehavior::Continue;
    }

Cleanup:
    return hr;
}