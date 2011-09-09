#include "SolidColorBrush.h"
#include "DelegatingPropertyInformation.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"

static ColorF DefaultColor(1, 1, 1, 1);

//
// Property Defaults
//
DEFINE_GET_DEFAULT( Color, CColorFValue, DefaultColor );

//
// Properties
//
CStaticProperty CSolidColorBrush::ColorProperty(L"Color", TypeIndex::ColorF, StaticPropertyFlags::None, NULL, &INSTANCE_CHANGE_CALLBACK( CSolidColorBrush, OnColorChanged ));

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CSolidColorBrush, OnColorChanged );

CSolidColorBrush::CSolidColorBrush(
    )
{
    m_Color.a = 0;
    m_Color.r = 0;
    m_Color.g = 0;
    m_Color.b = 0;
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

__checkReturn HRESULT
CSolidColorBrush::InternalSetColor(
    ColorF Color
    )
{
    HRESULT hr = S_OK;

    m_Color = Color;

    IFC(InvalidateVisualResource());

Cleanup:
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

    IFCPTR(pDevice);
    IFCPTR(pRenderTarget);
    IFCPTR(ppGraphicsBrush);

    //TODO: Cache per-render target?
    IFC(pRenderTarget->CreateSolidBrush(m_Color, ppGraphicsBrush));

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
CSolidColorBrush::SetValueInternal(
    __in CProperty* pProperty, 
    __in CObjectWithType* pValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    if(pProperty == &CSolidColorBrush::ColorProperty)
    {
        CColorFValue* pColorValue = (CColorFValue*)pValue;

        IFC(InternalSetColor(pColorValue->GetValue()));
    }
    else
    {
        IFC(CBrush::SetValueInternal(pProperty, pValue));
    }

Cleanup:
    return hr;
}

__override __checkReturn HRESULT
CSolidColorBrush::GetValueInternal(
    __in CProperty* pProperty,
    __deref_out CObjectWithType** ppValue
    )
{
    HRESULT hr = S_OK;
    CColorFValue* pColorValue = NULL;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(pProperty  == &CSolidColorBrush::ColorProperty)
    {
        IFC(CColorFValue::Create(m_Color, &pColorValue));
        
        *ppValue = pColorValue;
        pColorValue = NULL;
    }
    else
    {
        IFC(CBrush::GetValueInternal(pProperty, ppValue));
    }

Cleanup:
    ReleaseObject(pColorValue);
    
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