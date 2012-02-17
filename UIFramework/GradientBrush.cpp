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
CStaticProperty CGradientBrush::GradientStopsProperty(TypeIndex::GradientBrush, GradientBrushProperties::GradientStops, L"GradientStops", TypeIndex::GradientStop, StaticPropertyFlags::Content | StaticPropertyFlags::Collection | StaticPropertyFlags::ReadOnly, &GET_DEFAULT( GradientStops ), &INSTANCE_CHANGE_CALLBACK( CGradientBrush, OnGradientStopsChanged ));

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CGradientBrush, OnGradientStopsChanged );

CGradientBrush::CGradientBrush(
    ) 
    : m_GradientStops(NULL)
{
}

CGradientBrush::~CGradientBrush(
    )
{
    ReleaseObject(m_GradientStops);
}

__checkReturn HRESULT 
CGradientBrush::Initialize(
    __in CProviders* pProviders
    )
{
    HRESULT hr = S_OK;

    IFC(CGradientStopCollection::Create(&m_GradientStops));

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
CGradientBrush::SetValueInternal(
    __in CProperty* pProperty, 
    __in CObjectWithType* pValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    //if(pProperty == &CGradientBrush::GradientStopsProperty)
    //{
    //}
    //else
    //{
        IFC(CBrush::SetValueInternal(pProperty, pValue));
    //}

Cleanup:
    return hr;
}

__override __checkReturn HRESULT
CGradientBrush::GetValueInternal(
    __in CProperty* pProperty,
    __deref_out_opt CObjectWithType** ppValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(pProperty  == &CGradientBrush::GradientStopsProperty)
    {
        *ppValue = m_GradientStops;
        AddRefObject(m_GradientStops);
    }
    else
    {
        IFC(CBrush::GetValueInternal(pProperty, ppValue));
    }

Cleanup:
    return hr;
}