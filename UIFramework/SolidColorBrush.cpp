#include "SolidColorBrush.h"
#include "DelegatingPropertyInformation.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"

//
// Properties
//
namespace SolidColorBrushProperties
{
    enum Value
    {
        Color
    };
}

const ColorF DefaultColor(1, 1, 1, 1);

//
// Property Defaults
//
DEFINE_GET_DEFAULT( Color, ColorF, DefaultColor );

//
// Properties
//
CStaticProperty CSolidColorBrush::ColorProperty(TypeIndex::SolidColorBrush, SolidColorBrushProperties::Color, L"Color", TypeIndex::ColorF, StaticPropertyFlags::None, NULL, &INSTANCE_CHANGE_CALLBACK( CSolidColorBrush, OnColorChanged ));

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CSolidColorBrush, OnColorChanged );

CSolidColorBrush::CSolidColorBrush(
    )
{
}

CSolidColorBrush::~CSolidColorBrush(
    )
{
}

__checkReturn HRESULT 
CSolidColorBrush::Initialize(
    __in CProviders* pProviders
    )
{
    HRESULT hr = S_OK;

    return hr;
}

__override __checkReturn HRESULT
CSolidColorBrush::GetGraphicsBrush(
    __in CGraphicsDevice* pDevice,
    __in CRenderTarget* pRenderTarget,
    __deref_out CGraphicsBrush** ppGraphicsBrush
    )
{
    HRESULT hr = S_OK;
    ColorF brushColor;

    IFCPTR(pDevice);
    IFCPTR(pRenderTarget);
    IFCPTR(ppGraphicsBrush);

    IFC(GetBasicTypeEffectiveValue(&ColorProperty, &brushColor));

    //TODO: Cache per-render target?
    IFC(pRenderTarget->CreateSolidBrush(brushColor, ppGraphicsBrush));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CSolidColorBrush::CreatePropertyInformation(
    __deref_out CPropertyInformation** ppInformation
    )
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingPropertyInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &ColorProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));
    IFC(CBrush::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingPropertyInformation));

    *ppInformation = pDelegatingPropertyInformation;
    pDelegatingPropertyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingPropertyInformation);

    return hr;
}

__override __checkReturn HRESULT 
CSolidColorBrush::GetLayeredValue(
    __in CProperty* pProperty,
    __deref_out CLayeredValue** ppLayeredValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    if (pProperty->GetOwningType() == TypeIndex::SolidColorBrush)
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        switch(pStaticProperty->GetLocalIndex())
        {
            case SolidColorBrushProperties::Color:
                {
                    *ppLayeredValue = &m_Color;
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
        IFC_NOTRACE(CBrush::GetLayeredValue(pProperty, ppLayeredValue));
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT
CSolidColorBrush::OnColorChanged(
    __in_opt CObjectWithType* pOldValue, 
    __in_opt CObjectWithType* pNewValue
    )
{
    HRESULT hr = S_OK;

    IFC(InvalidateVisualResource());

Cleanup:
    return hr;
}