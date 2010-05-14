#include "LinearGradientBrush.h"
#include "DelegatingPropertyInformation.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"

Point2F DefaultStartPoint = { 0, 0 };
Point2F DefaultEndPoint = { 1, 1 };

//
// Property Defaults
//
DEFINE_GET_DEFAULT( StartPoint, CPoint2FValue, DefaultStartPoint );
DEFINE_GET_DEFAULT( EndPoint, CPoint2FValue, DefaultEndPoint );

//
// Properties
//
CStaticProperty CLinearGradientBrush::StartPointProperty( L"StartPoint", TypeIndex::Point2F, StaticPropertyFlags::None, &GET_DEFAULT( StartPoint ), &INSTANCE_CHANGE_CALLBACK( CLinearGradientBrush, OnStartPointChanged ) );
CStaticProperty CLinearGradientBrush::EndPointProperty( L"EndPoint", TypeIndex::Point2F, StaticPropertyFlags::None, &GET_DEFAULT( EndPoint ), &INSTANCE_CHANGE_CALLBACK( CLinearGradientBrush, OnStartPointChanged ) );

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CLinearGradientBrush, OnStartPointChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CLinearGradientBrush, OnEndPointChanged );

CLinearGradientBrush::CLinearGradientBrush()
{
    m_StartPoint.x = 0;
    m_StartPoint.y = 0;

    m_EndPoint.x = 1;
    m_EndPoint.y = 1;
}

CLinearGradientBrush::~CLinearGradientBrush()
{
}

HRESULT CLinearGradientBrush::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFC(CGradientBrush::Initialize(pProviders));

Cleanup:
    return hr;
}

HRESULT CLinearGradientBrush::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingProperyInformation = NULL;

    IFCPTR(ppInformation);

    CStaticProperty* Properties[] = 
    {
        &StartPointProperty,
        &EndPointProperty    
    };

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));
    IFC(CGradientBrush::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingProperyInformation));

    *ppInformation = pDelegatingProperyInformation;
    pDelegatingProperyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingProperyInformation);

    return hr;
}

HRESULT CLinearGradientBrush::SetValueInternal(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    if(pProperty == &CLinearGradientBrush::StartPointProperty)
    {
        CPoint2FValue* pPoint = NULL;

        IFC(CastType(pValue, &pPoint));

        m_StartPoint = pPoint->GetValue();

        IFC(InvalidateBrush());
    }
    else if(pProperty == &CLinearGradientBrush::EndPointProperty)
    {
        CPoint2FValue* pPoint = NULL;

        IFC(CastType(pValue, &pPoint));

        m_EndPoint = pPoint->GetValue();

        IFC(InvalidateBrush());
    }
    else
    {
        IFC(CGradientBrush::SetValueInternal(pProperty, pValue));
    }

Cleanup:
    return hr;
}

HRESULT CLinearGradientBrush::GetValue(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;
    CPoint2FValue* pPoint = NULL;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(pProperty == &CLinearGradientBrush::StartPointProperty)
    {
        IFC(CPoint2FValue::Create(m_StartPoint, &pPoint));

        *ppValue = pPoint;
        pPoint = NULL;
    }
    else if(pProperty == &CLinearGradientBrush::EndPointProperty)
    {
        IFC(CPoint2FValue::Create(m_EndPoint, &pPoint));

        *ppValue = pPoint;
        pPoint = NULL;
    }
    else
    {
        IFC(CGradientBrush::GetValue(pProperty, ppValue));
    }

Cleanup:
    return hr;
}

HRESULT CLinearGradientBrush::GetGraphicsBrush(CGraphicsDevice* pGraphicsDevice, CRenderTarget* pRenderTarget, CGraphicsBrush** ppGraphicsBrush)
{
    HRESULT hr = S_OK;
    GradientStop* pGradientStops = NULL;

    IFCPTR(pGraphicsDevice);
    IFCPTR(pRenderTarget);
    IFCPTR(ppGraphicsBrush);

    IFCPTR(m_GradientStops);
    IFCEXPECT(m_GradientStops->GetCount() > 0);

    pGradientStops = new GradientStop[m_GradientStops->GetCount()];

    for(UINT32 i = 0; i < m_GradientStops->GetCount(); i++)
    {
        CGradientStop* pGradientStop = m_GradientStops->GetAtIndex(i);

        pGradientStops[i].color = pGradientStop->GetColor();
        pGradientStops[i].position = pGradientStop->GetOffset();
    }

    //TODO: Cache per-render target?
    IFC(pRenderTarget->CreateLinearGradientBrush(m_StartPoint, m_EndPoint, pGradientStops, m_GradientStops->GetCount(), ppGraphicsBrush));

Cleanup:
    delete [] pGradientStops;

    return hr;
}

HRESULT CLinearGradientBrush::OnStartPointChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    IFC(InvalidateBrush());

Cleanup:
    return hr;
}

HRESULT CLinearGradientBrush::OnEndPointChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    IFC(InvalidateBrush());

Cleanup:
    return hr;
}