#include "GradientBrush.h"
#include "DelegatingPropertyInformation.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( GradientStops );

//
// Properties
//
CStaticProperty CGradientBrush::GradientStopsProperty( L"GradientStops", TypeIndex::GradientStop, StaticPropertyFlags::Content | StaticPropertyFlags::Collection | StaticPropertyFlags::ReadOnly, &GET_DEFAULT( GradientStops ), &INSTANCE_CHANGE_CALLBACK( CGradientBrush, OnGradientStopsChanged ) );

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CGradientBrush, OnGradientStopsChanged );

CGradientBrush::CGradientBrush() : m_GradientStops(NULL)
{
}

CGradientBrush::~CGradientBrush()
{
    ReleaseObject(m_GradientStops);
}

HRESULT CGradientBrush::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFC(CGradientStopCollection::Create(&m_GradientStops));

Cleanup:
    return hr;
}

HRESULT CGradientBrush::OnGradientStopsChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    IFC(InvalidateBrush());

    //TODO: Disconnect and reconnect to the new collection?

Cleanup:
    return hr;
}

HRESULT CGradientBrush::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingProperyInformation = NULL;
    
    CStaticProperty* Properties[] = 
    {
        &GradientStopsProperty
    };    

    IFCPTR(ppInformation);

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

HRESULT CGradientBrush::SetValueInternal(CProperty* pProperty, CObjectWithType* pValue)
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

HRESULT CGradientBrush::GetValue(CProperty* pProperty, CObjectWithType** ppValue)
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
        IFC(CBrush::GetValue(pProperty, ppValue));
    }

Cleanup:
    return hr;
}