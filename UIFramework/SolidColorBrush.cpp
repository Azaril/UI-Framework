#include "SolidColorBrush.h"
#include "DelegatingPropertyInformation.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"

//
// Properties
//
CStaticProperty CSolidColorBrush::ColorProperty( L"Color", TypeIndex::ColorF, StaticPropertyFlags::None );

CSolidColorBrush::CSolidColorBrush()
{
    m_Color.a = 0;
    m_Color.r = 0;
    m_Color.g = 0;
    m_Color.b = 0;
}

CSolidColorBrush::~CSolidColorBrush()
{
}

HRESULT CSolidColorBrush::Initialize()
{
    HRESULT hr = S_OK;

    return hr;
}

HRESULT CSolidColorBrush::InternalSetColor(ColorF Color)
{
    HRESULT hr = S_OK;

    m_Color = Color;

    IFC(InvalidateBrush());

Cleanup:
    return hr;
}

HRESULT CSolidColorBrush::GetGraphicsBrush(CGraphicsDevice* pDevice, CRenderTarget* pRenderTarget, CGraphicsBrush** ppGraphicsBrush)
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

HRESULT CSolidColorBrush::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingProperyInformation = NULL;

    IFCPTR(ppInformation);

    CStaticProperty* Properties[] = 
    {
        &ColorProperty
    };

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));
    IFC(CBrush::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingProperyInformation));

    *ppInformation = pDelegatingProperyInformation;
    pDelegatingProperyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingProperyInformation);

    return hr;
}

HRESULT CSolidColorBrush::SetValue(CProperty* pProperty, CObjectWithType* pValue)
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
        IFC(CBrush::SetValue(pProperty, pValue));
    }

Cleanup:
    return hr;
}

HRESULT CSolidColorBrush::GetValue(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(pProperty  == &CSolidColorBrush::ColorProperty)
    {
        IFC(E_NOTIMPL);
    }
    else
    {
        IFC(CBrush::GetValue(pProperty, ppValue));
    }

Cleanup:
    return hr;
}