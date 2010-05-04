#include "Brush.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"

CStaticProperty BrushProperties[] = 
{
    CStaticProperty( L"Opacity", TypeIndex::Float, StaticPropertyFlags::None )
};

namespace BrushPropertyIndex
{
    enum Value
    {
        Opacity
    };
}

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

    return hr;
}

HRESULT CBrush::OnVisualDetach(CVisualDetachContext& Context)
{
    HRESULT hr = S_OK;

    return hr;
}

HRESULT CBrush::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;

    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(BrushProperties, ARRAYSIZE(BrushProperties), &pStaticInformation));

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

    // Check if the property is a static property.
    if(pProperty >= BrushProperties && pProperty < BrushProperties + ARRAYSIZE(BrushProperties))
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        UINT32 Index = (pStaticProperty - BrushProperties);
        
        switch(Index)
        {
            case BrushPropertyIndex::Opacity:
                {
                    IFCEXPECT(pValue->IsTypeOf(TypeIndex::Float));

                    CFloatValue* pFloat = (CFloatValue*)pValue;

                    //TODO: Set opacity!

                    __debugbreak();

                    break;
                }

            default:
                {
                    IFC(E_FAIL);
                }
        }
    }
    else
    {
        IFC(CPropertyObject::SetValue(pProperty, pValue));
    }

Cleanup:
    return hr;
}

HRESULT CBrush::GetValue(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    //TODO: Ensure this property actually belongs to this object.

    //TODO: Looking up other than by name would be much better.
    //if(wcscmp(pProperty->GetName(), L"Opacity") == 0)
    //{
    //    IFCEXPECT(pValue->IsTypeOf(TypeIndex::Float));

    //    CFloatValue* pFloat = (CFloatValue*)pValue;

    //    //TODO: Set opacity.
    //}
    //else
    {
        IFC(CPropertyObject::GetValue(pProperty, ppValue));
    }

Cleanup:
    return hr;
}