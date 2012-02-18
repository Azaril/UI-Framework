#include "GradientStop.h"
#include "BasicTypes.h"

//
// Properties
//
namespace GradientStopProperties
{
    enum Value
    {
        Color,
        Offset
    };
}

const ColorF DefaultColor(1, 1, 1, 1);

//
// Property Defaults
//
DEFINE_GET_DEFAULT( Color, ColorF, DefaultColor );
DEFINE_GET_DEFAULT( Offset, FLOAT, 0 );

//
// Properties
//
CStaticProperty CGradientStop::ColorProperty(TypeIndex::GradientStop, GradientStopProperties::Color, L"Color", TypeIndex::ColorF, StaticPropertyFlags::None, NULL, &INSTANCE_CHANGE_CALLBACK( CGradientStop, OnColorChanged ));
CStaticProperty CGradientStop::OffsetProperty(TypeIndex::GradientStop, GradientStopProperties::Offset, L"Offset", TypeIndex::Float, StaticPropertyFlags::None, NULL, &INSTANCE_CHANGE_CALLBACK( CGradientStop, OnOffsetChanged ));

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CGradientStop, OnColorChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CGradientStop, OnOffsetChanged );

CGradientStop::CGradientStop(
    )
{
}

CGradientStop::~CGradientStop(
    )
{
}

__checkReturn HRESULT 
CGradientStop::Initialize(
    __in CProviders* pProviders
    )
{
    HRESULT hr = S_OK;

    return hr;
}

__checkReturn HRESULT 
CGradientStop::OnColorChanged(
    __in_opt CObjectWithType* pOldValue,
    __in_opt CObjectWithType* pNewValue
    )
{
    HRESULT hr = S_OK;

    //TODO: Invalidate parent?

    return hr;
}

__checkReturn HRESULT 
CGradientStop::OnOffsetChanged(
    __in_opt CObjectWithType* pOldValue, 
    __in_opt CObjectWithType* pNewValue
    )
{
    HRESULT hr = S_OK;

    //TODO: Invalidate parent?

    return hr;
}

__checkReturn HRESULT 
CGradientStop::CreatePropertyInformation(
    __deref_out CPropertyInformation** ppInformation
    )
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &ColorProperty,
        &OffsetProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));

    *ppInformation = pStaticInformation;
    pStaticInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);

    return hr;
}

__override __checkReturn HRESULT 
CGradientStop::GetLayeredValue(
    __in CProperty* pProperty,
    __deref_out CLayeredValue** ppLayeredValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    if (pProperty->GetOwningType() == TypeIndex::GradientStop)
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        switch(pStaticProperty->GetLocalIndex())
        {
            case GradientStopProperties::Color:
                {
                    *ppLayeredValue = &m_Color;
                    break;
                }

            case GradientStopProperties::Offset:
                {
                    *ppLayeredValue = &m_Offset;
                    break;
                }

            default:
                {
                    IFC(E_UNEXPECTED);
                }
        }
    }
    else
    {
        IFC_NOTRACE(CPropertyObject::GetLayeredValue(pProperty, ppLayeredValue));
    }

Cleanup:
    return hr;
}