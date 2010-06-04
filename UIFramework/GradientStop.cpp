#include "GradientStop.h"
#include "BasicTypes.h"

static ColorF DefaultColor(1, 1, 1, 1);

//
// Property Defaults
//
DEFINE_GET_DEFAULT( Color, CColorFValue, DefaultColor );
DEFINE_GET_DEFAULT( Offset, CFloatValue, 0 );

//
// Properties
//
CStaticProperty CGradientStop::ColorProperty( L"Color", TypeIndex::ColorF, StaticPropertyFlags::None, NULL, &INSTANCE_CHANGE_CALLBACK( CGradientStop, OnColorChanged ) );
CStaticProperty CGradientStop::OffsetProperty( L"Offset", TypeIndex::Float, StaticPropertyFlags::None, NULL, &INSTANCE_CHANGE_CALLBACK( CGradientStop, OnOffsetChanged ) );

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CGradientStop, OnColorChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CGradientStop, OnOffsetChanged );

CGradientStop::CGradientStop() : m_Color(DefaultColor),
                                 m_Offset(0)
{
}

CGradientStop::~CGradientStop()
{
}

HRESULT CGradientStop::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    return hr;
}

ColorF CGradientStop::GetColor()
{
    return m_Color;
}

FLOAT CGradientStop::GetOffset()
{
    return m_Offset;
}

HRESULT CGradientStop::OnColorChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    //TODO: Invalidate parent?

    return hr;
}

HRESULT CGradientStop::OnOffsetChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    //TODO: Invalidate parent?

    return hr;
}

HRESULT CGradientStop::CreatePropertyInformation(CPropertyInformation** ppInformation)
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

HRESULT CGradientStop::SetValueInternal(CProperty* pProperty, CObjectWithType* pValue)
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

HRESULT CGradientStop::GetValue(CProperty* pProperty, CObjectWithType** ppValue)
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
        IFC(CPropertyObject::GetValue(pProperty, ppValue));
    }

Cleanup:
    ReleaseObject(pColorValue);
    ReleaseObject(pFloatValue);

    return hr;
}