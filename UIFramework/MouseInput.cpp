#include "MouseInput.h"

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

HRESULT CMouseInputHitTestCallback::ItemHit(CVisual* pVisual, HitTestResultBehavior::Value* pResultBehavior)
{
    HRESULT hr = S_OK;

    IFCPTR(pVisual);
    IFCPTR(pResultBehavior);

    if(pVisual->IsTypeOf(TypeIndex::UIElement))
    {
        *pResultBehavior = HitTestResultBehavior::Stop;
    }
    else
    {
        *pResultBehavior = HitTestResultBehavior::Continue;
    }

Cleanup:
    return S_OK;
}