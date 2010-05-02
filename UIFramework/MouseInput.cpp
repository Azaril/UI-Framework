#include "MouseInput.h"
#include "RoutedEventArgs.h"
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

CMouseDownHitTestCallback::CMouseDownHitTestCallback(MouseButton::Value Button) : m_Button(Button)
{
}

HRESULT CMouseDownHitTestCallback::ItemHit(CVisual* pVisual, HitTestResultBehavior::Value* pResultBehavior)
{
    HRESULT hr = S_OK;
    CRoutedEventArgs* pEventArgs = NULL;

    IFCPTR(pVisual);
    IFCPTR(pResultBehavior);

    if(pVisual->IsTypeOf(TypeIndex::UIElement))
    {
        CUIElement* pElement = (CUIElement*)pVisual;

        IFC(CRoutedEventArgs::Create(&CUIElement::MouseDownEvent, &pEventArgs));

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