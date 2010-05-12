#include "Button.h"

CButton::CButton()
{
}

CButton::~CButton()
{
}

HRESULT CButton::Initialize()
{
    HRESULT hr = S_OK;

    IFC(CButtonBase::Initialize());

Cleanup:
    return hr;
}

HRESULT CButton::HitTest(Point2F LocalPoint, CHitTestResult** ppHitTestResult)
{
    HRESULT hr = S_OK;
    CHitTestResult* pVisualHitTestResult = NULL;

    IFCPTR(ppHitTestResult);

    //IFC(m_BorderVisual->HitTest(LocalPoint, &pVisualHitTestResult));

    if(pVisualHitTestResult)
    {
        IFC(CHitTestResult::Create(this, ppHitTestResult));
    }

    *ppHitTestResult = pVisualHitTestResult;

Cleanup:
    ReleaseObject(pVisualHitTestResult);

    return hr;
}