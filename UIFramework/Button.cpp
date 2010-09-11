#include "Button.h"

CButton::CButton()
{
}

CButton::~CButton()
{
}

HRESULT CButton::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFC(CButtonBase::Initialize(pProviders));

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

//
// CButton
//
extern "C" __declspec(dllexport)
TypeIndex::Value CButton_TypeIndex()
{
    return TypeIndex::Button;
}

extern "C" __declspec(dllexport)
CButtonBase* CButton_CastTo_CButtonBase(CButton* pButton)
{
    return pButton;
}

extern "C" __declspec(dllexport)
CButton* CObjectWithType_CastTo_CButton(CObjectWithType* pObject)
{
    return (pObject->IsTypeOf(TypeIndex::Button)) ? (CButton*)pObject : NULL;
}