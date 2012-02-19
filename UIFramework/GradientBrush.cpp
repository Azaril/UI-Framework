#include "GradientBrush.h"
#include "DelegatingPropertyInformation.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"

//
// Properties
//
namespace GradientBrushProperties
{
    enum Value
    {
        GradientStops
    };
}

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( GradientStops );

//
// Properties
//
CStaticProperty CGradientBrush::GradientStopsProperty(TypeIndex::GradientBrush, GradientBrushProperties::GradientStops, L"GradientStops", TypeIndex::GradientStopCollection, StaticPropertyFlags::Content | StaticPropertyFlags::Collection, &GET_DEFAULT( GradientStops ), &INSTANCE_CHANGE_CALLBACK( CGradientBrush, OnGradientStopsChanged ));

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CGradientBrush, OnGradientStopsChanged );

CGradientBrush::CGradientBrush(
    )
{
}

CGradientBrush::~CGradientBrush(
    )
{
}

__checkReturn HRESULT 
CGradientBrush::Initialize(
    __in CProviders* pProviders
    )
{
    HRESULT hr = S_OK;

    IFC(CBrush::Initialize(pProviders));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CGradientBrush::OnGradientStopsChanged(
    __in CObjectWithType* pOldValue,
    __in CObjectWithType* pNewValue
    )
{
    HRESULT hr = S_OK;

    IFC(InvalidateVisualResource());

    //TODO: Disconnect and reconnect to the new collection?

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CGradientBrush::CreatePropertyInformation(
    __in CPropertyInformation **ppInformation
    )
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingPropertyInformation = NULL;
    
    CStaticProperty* Properties[] = 
    {
        &GradientStopsProperty
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
CGradientBrush::GetLayeredValue(
    __in CProperty* pProperty,
    __deref_out CLayeredValue** ppLayeredValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    if (pProperty->GetOwningType() == TypeIndex::GradientBrush)
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        switch(pStaticProperty->GetLocalIndex())
        {
            case GradientBrushProperties::GradientStops:
                {
                    *ppLayeredValue = &m_GradientStops;
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