#include "Brush.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"

CBrush::CBrush() : m_PropertyInformation(NULL)
{
}

CBrush::~CBrush()
{
    ReleaseObject(m_PropertyInformation);
}

HRESULT CBrush::OnVisualAttach(CVisualAttachContext& Context)
{
    HRESULT hr = S_OK;

    IFC(CVisualResource::OnVisualAttach(Context));

Cleanup:
    return hr;
}

HRESULT CBrush::OnVisualDetach(CVisualDetachContext& Context)
{
    HRESULT hr = S_OK;

    IFC(CVisualResource::OnVisualDetach(Context));

Cleanup:
    return hr;
}

HRESULT CBrush::InvalidateBrush()
{
    HRESULT hr = S_OK;
    CBrushInvalidatedNotification* pNotification = NULL;

    IFC(CBrushInvalidatedNotification::Create(this, &pNotification));

    IFC(NotifyParents(pNotification));

Cleanup:
    ReleaseObject(pNotification);

    return hr;
}

HRESULT CBrush::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;

    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(NULL, 0, &pStaticInformation));

    *ppInformation = pStaticInformation;
    pStaticInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);

    return hr;
}

HRESULT CBrush::SetValue(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    IFC(CPropertyObject::SetValue(pProperty, pValue));

Cleanup:
    return hr;
}

HRESULT CBrush::GetValue(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    IFC(CPropertyObject::GetValue(pProperty, ppValue));

Cleanup:
    return hr;
}




CBrushInvalidatedNotification::CBrushInvalidatedNotification() : m_Brush(NULL)
{
}

CBrushInvalidatedNotification::~CBrushInvalidatedNotification()
{
    ReleaseObject(m_Brush);
}

HRESULT CBrushInvalidatedNotification::Initialize(CBrush* pBrush)
{
    HRESULT hr = S_OK;

    IFCPTR(pBrush);

    m_Brush = pBrush;
    AddRefObject(m_Brush);

Cleanup:
    return hr;
}

VisualNotification::Value CBrushInvalidatedNotification::GetType()
{
    return VisualNotification::ChildBrushInvalidated;
}

CBrush* CBrushInvalidatedNotification::GetBrush()
{
    return m_Brush;
}