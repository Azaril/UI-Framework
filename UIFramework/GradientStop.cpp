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
CStaticProperty CGradientStop::ColorProperty(GradientStopProperties::Color, L"Color", TypeIndex::ColorF, StaticPropertyFlags::None, NULL, &INSTANCE_CHANGE_CALLBACK( CGradientStop, OnColorChanged ));
CStaticProperty CGradientStop::OffsetProperty(GradientStopProperties::Offset, L"Offset", TypeIndex::Float, StaticPropertyFlags::None, NULL, &INSTANCE_CHANGE_CALLBACK( CGradientStop, OnOffsetChanged ));

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CGradientStop, OnColorChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CGradientStop, OnOffsetChanged );

CGradientStop::CGradientStop(
    ) 
    : m_Color(DefaultColor)
    , m_Offset(0)
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

ColorF 
CGradientStop::GetColor(
    )
{
    return m_Color;
}

FLOAT 
CGradientStop::GetOffset(
    )
{
    return m_Offset;
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
CGradientStop::SetValueInternal(
    __in CProperty* pProperty, 
    __in CObjectWithType* pValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    if(pProperty == &CGradientStop::ColorProperty)
    {
        CColorFValue* pColorValue = NULL;

        IFC(CastType(pValue, &pColorValue));

        m_Color = pColorValue->GetValue();
    }
    else if(pProperty == &CGradientStop::OffsetProperty)
    {
        CFloatValue* pFloatValue = NULL;

        IFC(CastType(pValue, &pFloatValue));

        m_Offset = pFloatValue->GetValue();
    }
    else
    {
        IFC(CPropertyObject::SetValueInternal(pProperty, pValue));
    }

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CGradientStop::GetValueInternal(
    __in CProperty* pProperty, 
    __deref_out_opt CObjectWithType** ppValue
    )
{
    HRESULT hr = S_OK;
    CColorFValue* pColorValue = NULL;
    CFloatValue* pFloatValue = NULL;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(pProperty == &CGradientStop::ColorProperty)
    {
        IFC(CColorFValue::Create(m_Color, &pColorValue));

        *ppValue = pColorValue;
        pColorValue = NULL;
    }
    else if(pProperty == &CGradientStop::OffsetProperty)
    {
        IFC(CFloatValue::Create(m_Offset, &pFloatValue));

        *ppValue = pFloatValue;
        pFloatValue = NULL;
    }
    else
    {
        IFC(CPropertyObject::GetValueInternal(pProperty, ppValue));
    }

Cleanup:
    ReleaseObject(pColorValue);
    ReleaseObject(pFloatValue);

    return hr;
}