#include "LinearGradientBrush.h"
#include "DelegatingPropertyInformation.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"

Point2F DefaultStartPoint(0, 0);
Point2F DefaultEndPoint(1, 1);

//
// Property Defaults
//
DEFINE_GET_DEFAULT( StartPoint, CPoint2FValue, DefaultStartPoint );
DEFINE_GET_DEFAULT( EndPoint, CPoint2FValue, DefaultEndPoint );

//
// Properties
//
CStaticProperty CLinearGradientBrush::StartPointProperty(L"StartPoint", TypeIndex::Point2F, StaticPropertyFlags::None, &GET_DEFAULT( StartPoint ), &INSTANCE_CHANGE_CALLBACK( CLinearGradientBrush, OnStartPointChanged ));
CStaticProperty CLinearGradientBrush::EndPointProperty(L"EndPoint", TypeIndex::Point2F, StaticPropertyFlags::None, &GET_DEFAULT( EndPoint ), &INSTANCE_CHANGE_CALLBACK( CLinearGradientBrush, OnStartPointChanged ));

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CLinearGradientBrush, OnStartPointChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CLinearGradientBrush, OnEndPointChanged );

CLinearGradientBrush::CLinearGradientBrush(
    )
{
    m_StartPoint.x = 0;
    m_StartPoint.y = 0;

    m_EndPoint.x = 1;
    m_EndPoint.y = 1;
}

CLinearGradientBrush::~CLinearGradientBrush(
    )
{
}

__checkReturn HRESULT 
CLinearGradientBrush::Initialize(
    __in CProviders* pProviders
    )
{
    HRESULT hr = S_OK;

    IFC(CGradientBrush::Initialize(pProviders));

Cleanup:
    return hr;
}

__checkReturn HRESULT
CLinearGradientBrush::CreatePropertyInformation(
    __deref_out CPropertyInformation** ppInformation
    )
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingPropertyInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &StartPointProperty,
        &EndPointProperty    
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));
    IFC(CGradientBrush::CreatePropertyInformation(&pBaseInformation));
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
CLinearGradientBrush::SetValueInternal(
    __in CProperty* pProperty, 
    __in CObjectWithType* pValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    if(pProperty == &CLinearGradientBrush::StartPointProperty)
    {
        CPoint2FValue* pPoint = NULL;

        IFC(CastType(pValue, &pPoint));

        m_StartPoint = pPoint->GetValue();

        //TODO: Move this to property change handler...
        IFC(InvalidateVisualResource());
    }
    else if(pProperty == &CLinearGradientBrush::EndPointProperty)
    {
        CPoint2FValue* pPoint = NULL;

        IFC(CastType(pValue, &pPoint));

        m_EndPoint = pPoint->GetValue();

        //TODO: Move this to property change handler...
        IFC(InvalidateVisualResource());
    }
    else
    {
        IFC(CGradientBrush::SetValueInternal(pProperty, pValue));
    }

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CLinearGradientBrush::GetValueInternal(
    __in CProperty* pProperty, 
    __deref_out_opt CObjectWithType** ppValue
    )
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
        IFC(CGradientBrush::GetValueInternal(pProperty, ppValue));
    }

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CLinearGradientBrush::GetGraphicsBrush(
    __in CGraphicsDevice* pGraphicsDevice, 
    __in CRenderTarget* pRenderTarget, 
    __deref_out CGraphicsBrush** ppGraphicsBrush
    )
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

__checkReturn HRESULT
CLinearGradientBrush::OnStartPointChanged(
    __in_opt CObjectWithType* pOldValue, 
    __in_opt CObjectWithType* pNewValue
    )
{
    HRESULT hr = S_OK;

    IFC(InvalidateVisualResource());

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CLinearGradientBrush::OnEndPointChanged(
    __in_opt CObjectWithType* pOldValue, 
    __in_opt CObjectWithType* pNewValue
    )
{
    HRESULT hr = S_OK;

    IFC(InvalidateVisualResource());

Cleanup:
    return hr;
}