#include "LinearGradientBrush.h"
#include "DelegatingPropertyInformation.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"

//
// Properties
//
namespace LinearGradientBrushProperties
{
    enum Value
    {
        StartPoint,
        EndPoint
    };
}

const Point2F DefaultStartPoint(0, 0);
const Point2F DefaultEndPoint(1, 1);

//
// Property Defaults
//
DEFINE_GET_DEFAULT( StartPoint, Point2F, DefaultStartPoint );
DEFINE_GET_DEFAULT( EndPoint, Point2F, DefaultEndPoint );

//
// Properties
//
CStaticProperty CLinearGradientBrush::StartPointProperty(TypeIndex::LinearGradientBrush, LinearGradientBrushProperties::StartPoint, L"StartPoint", TypeIndex::Point2F, StaticPropertyFlags::None, &GET_DEFAULT( StartPoint ), &INSTANCE_CHANGE_CALLBACK( CLinearGradientBrush, OnStartPointChanged ));
CStaticProperty CLinearGradientBrush::EndPointProperty(TypeIndex::LinearGradientBrush, LinearGradientBrushProperties::EndPoint, L"EndPoint", TypeIndex::Point2F, StaticPropertyFlags::None, &GET_DEFAULT( EndPoint ), &INSTANCE_CHANGE_CALLBACK( CLinearGradientBrush, OnStartPointChanged ));

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CLinearGradientBrush, OnStartPointChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CLinearGradientBrush, OnEndPointChanged );

CLinearGradientBrush::CLinearGradientBrush(
    )
{
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
CLinearGradientBrush::GetLayeredValue(
    __in CProperty* pProperty,
    __deref_out CLayeredValue** ppLayeredValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    if (pProperty->GetOwningType() == TypeIndex::LinearGradientBrush)
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        switch(pStaticProperty->GetLocalIndex())
        {
            case LinearGradientBrushProperties::StartPoint:
                {
                    *ppLayeredValue = &m_StartPoint;
                    break;
                }

            case LinearGradientBrushProperties::EndPoint:
                {
                    *ppLayeredValue = &m_EndPoint;
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
        IFC_NOTRACE(CGradientBrush::GetLayeredValue(pProperty, ppLayeredValue));
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
    CGradientStopCollection* pGradientStopCollection = NULL;
    GradientStop* pGradientStops = NULL;
    Point2F startPoint;
    Point2F endPoint;

    IFCPTR(pGraphicsDevice);
    IFCPTR(pRenderTarget);
    IFCPTR(ppGraphicsBrush);

    IFC(GetTypedEffectiveValue(&GradientStopsProperty, &pGradientStopCollection));

    IFCPTR(pGradientStopCollection);

    UINT32 gradientStopCount = pGradientStopCollection->GetCount();

    {        
        IFCEXPECT(gradientStopCount > 0);

        pGradientStops = new GradientStop[gradientStopCount];

        for(UINT32 i = 0; i < gradientStopCount; i++)
        {
            CGradientStop* pGradientStop = pGradientStopCollection->GetAtIndex(i);

            IFC(pGradientStop->GetBasicTypeEffectiveValue(&CGradientStop::ColorProperty, &pGradientStops[i].color));
            IFC(pGradientStop->GetBasicTypeEffectiveValue(&CGradientStop::OffsetProperty, &pGradientStops[i].position));
        }

        IFC(GetBasicTypeEffectiveValue(&StartPointProperty, &startPoint));
        IFC(GetBasicTypeEffectiveValue(&EndPointProperty, &endPoint));

        //TODO: Cache per-render target?
        IFC(pRenderTarget->CreateLinearGradientBrush(startPoint, endPoint, pGradientStops, gradientStopCount, ppGraphicsBrush));
    }

Cleanup:
    delete [] pGradientStops;
    ReleaseObject(pGradientStopCollection);

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