#include "Brush.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"

StaticClassProperty BrushProperties[] =
{
    { L"Opacity", FALSE, TypeIndex::Float }
};

StaticClassProperties BrushPropertyInformation =
{
    BrushProperties,
    ARRAYSIZE(BrushProperties)
};

CBrush::CBrush() : m_PropertyInformation(NULL)
{
}

CBrush::~CBrush()
{
    ReleaseObject(m_PropertyInformation);
}

HRESULT CBrush::GetPropertyInformation(CPropertyInformation** ppInformation)
{
    HRESULT hr = S_OK;

    IFCPTR(ppInformation);

    if(m_PropertyInformation == NULL)
    {
        IFC(CreatePropertyInformation(&m_PropertyInformation));
    }

    *ppInformation = m_PropertyInformation;
    AddRefObject(m_PropertyInformation);

Cleanup:
    return hr;
}

HRESULT CBrush::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;

    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(&BrushPropertyInformation, &pStaticInformation));

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

    //TODO: Ensure this property actually belongs to this object.

    //TODO: Looking up other than by name would be much better.
    if(wcscmp(pProperty->GetName(), L"Opacity") == 0)
    {
        IFCEXPECT(pValue->IsTypeOf(TypeIndex::Float));

        CFloatValue* pFloat = (CFloatValue*)pValue;

        //TODO: Set opacity.
    }
    else
    {
        IFC(E_FAIL);
    }

Cleanup:
    return hr;
}