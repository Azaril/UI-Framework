#include "HitTesting.h"

__checkReturn HRESULT 
WalkVisualTreeForHitTesting(
    const Point2F& LocalPoint, 
    __in CVisual* pParent, 
    __in_opt CHitTestFilter* pFilter,
    __in CHitTestCallback* pCallback
    )
{
    HRESULT hr = S_OK;
    CTransform* pTransform = NULL;
    CHitTestResult* pHitTestResult = NULL;
    HitTestFilterBehavior::Value FilterBehavior = HitTestFilterBehavior::Continue;
    HitTestResultBehavior::Value ResultBehavior = HitTestResultBehavior::Continue;

    if(pParent != NULL)
    {
        if(pFilter != NULL)
        {
            IFC(pFilter->Filter(pParent, &FilterBehavior));
        }

        if(FilterBehavior == HitTestFilterBehavior::Continue || FilterBehavior == HitTestFilterBehavior::ContinueSkipSelf)
        {
            UINT32 ChildCount = pParent->GetVisualChildCount();

            for(UINT32 i = 0; i < ChildCount; i++)
            {
                Point2F ChildPoint;
                CVisual* pChild = pParent->GetVisualChild(ChildCount - i - 1);

                IFC(pChild->TransformToParent(&pTransform));

                IFC(pTransform->TransformPoint(LocalPoint, &ChildPoint));

                IFC(WalkVisualTreeForHitTesting(ChildPoint, pChild, pFilter, pCallback));

                ReleaseObject(pTransform);

                if(hr == S_FALSE)
                {
                    FilterBehavior = HitTestFilterBehavior::Stop;

                    break;
                }
            }
        }

        if(FilterBehavior == HitTestFilterBehavior::Continue || FilterBehavior == HitTestFilterBehavior::ContinueSkipChildren)
        {
            IFC(pParent->HitTest(LocalPoint, &pHitTestResult));

            if(pHitTestResult)
            {
                IFC(pCallback->ItemHit(pParent, &ResultBehavior));
            }
        }
    }

    if(FilterBehavior == HitTestFilterBehavior::Stop || ResultBehavior == HitTestResultBehavior::Stop)
    {
        hr = S_FALSE;
    }

Cleanup:
    ReleaseObject(pTransform);
    ReleaseObject(pHitTestResult);

    return hr;
}

__checkReturn HRESULT 
HitTestTree(CVisual* pRootVisual, const Point2F& LocalPoint, CHitTestFilter* pFilter, CHitTestCallback* pCallback)
{
    HRESULT hr = S_OK;

    IFCPTR(pCallback);

    IFC(WalkVisualTreeForHitTesting(LocalPoint, pRootVisual, pFilter, pCallback));

Cleanup:
    return hr;
}