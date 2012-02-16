#include "RangeBase.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"
#include "RoutedEventInformation.h"
#include "DelegatingRoutedEventInformation.h"

//
// Properties
//
namespace RangeBaseProperties
{
    enum Value
    {
        Maximum,
        Minimum,
        Value
    };
}

//
// Property Defaults
//
DEFINE_GET_DEFAULT( Maximum, FLOAT, 100.0f );
DEFINE_GET_DEFAULT( Minimum, FLOAT, 0.0f );
DEFINE_GET_DEFAULT( Value, FLOAT, 0.0f );

//
// Properties
//
CStaticProperty CRangeBase::MaximumProperty(RangeBaseProperties::Maximum, L"Maximum", TypeIndex::Float, StaticPropertyFlags::None, &GET_DEFAULT( Maximum ), &INSTANCE_CHANGE_CALLBACK( CRangeBase, OnMaximumChanged ));
CStaticProperty CRangeBase::MinimumProperty(RangeBaseProperties::Minimum, L"Minimum", TypeIndex::Float, StaticPropertyFlags::None, &GET_DEFAULT( Minimum ), &INSTANCE_CHANGE_CALLBACK( CRangeBase, OnMinimumChanged ));
CStaticProperty CRangeBase::ValueProperty(RangeBaseProperties::Value, L"Value", TypeIndex::Float, StaticPropertyFlags::None, &GET_DEFAULT( Value ), &INSTANCE_CHANGE_CALLBACK( CRangeBase, OnValueChanged ));

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CRangeBase, OnMaximumChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CRangeBase, OnMinimumChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CRangeBase, OnValueChanged );

//
// Events
//
CStaticRoutedEvent CRangeBase::ValueChangedEvent(L"ValueChanged", RoutingStrategy::Bubbling);

CRangeBase::CRangeBase() : m_Maximum(this, &CRangeBase::MaximumProperty),
                           m_Minimum(this, &CRangeBase::MinimumProperty),
                           m_Value(this, &CRangeBase::ValueProperty)
{
}

CRangeBase::~CRangeBase()
{
}

HRESULT CRangeBase::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFCPTR(pProviders);

    IFC(CControl::Initialize(pProviders));

Cleanup:
    return hr;
}

HRESULT CRangeBase::GetLayeredValue(CProperty* pProperty, CLayeredValue** ppLayeredValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    //TODO: Make this a lookup table rather than requiring a comparison per property.
    if(pProperty == &CRangeBase::MaximumProperty)
    {
        *ppLayeredValue = &m_Maximum;
    }
    else if(pProperty == &CRangeBase::MinimumProperty)
    {
        *ppLayeredValue = &m_Minimum;
    }
    else if(pProperty == &CRangeBase::ValueProperty)
    {
        *ppLayeredValue = &m_Value;
    }
    else
    {
        hr = CControl::GetLayeredValue(pProperty, ppLayeredValue);
    }

Cleanup:
    return hr;
}

HRESULT CRangeBase::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingPropertyInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &MaximumProperty,
        &MinimumProperty,
        &ValueProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation))
    IFC(CControl::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingPropertyInformation));

    *ppInformation = pDelegatingPropertyInformation;
    pDelegatingPropertyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingPropertyInformation);

    return hr;
}

HRESULT CRangeBase::CreateEventInformation(CEventInformation** ppInformation)
{
    HRESULT hr = S_OK;
    CRoutedEventInformation* pEventInformation = NULL;
    CEventInformation* pBaseInformation = NULL;
    CDelegatingRoutedEventInformation* pDelegatingEventInformation = NULL;

    CRoutedEvent* Events[] = 
    {
        &ValueChangedEvent
    };
    
    IFCPTR(ppInformation);

    IFC(CRoutedEventInformation::Create(Events, ARRAYSIZE(Events), &pEventInformation));
    IFC(CControl::CreateEventInformation(&pBaseInformation));
    IFC(CDelegatingRoutedEventInformation::Create(pEventInformation, pBaseInformation, &pDelegatingEventInformation));

    *ppInformation = pDelegatingEventInformation;
    pDelegatingEventInformation = NULL;

Cleanup:
    ReleaseObject(pEventInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingEventInformation);

    return hr;
}

HRESULT CRangeBase::OnMaximumChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    return hr;
}

HRESULT CRangeBase::OnMinimumChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    return hr;
}

HRESULT CRangeBase::OnValueChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;
    CRoutedEventArgs* pValueChangedEventArgs = NULL;

    IFC(CRoutedEventArgs::Create(&ValueChangedEvent, &pValueChangedEventArgs));

    IFC(RaiseEvent(pValueChangedEventArgs));

Cleanup:
    ReleaseObject(pValueChangedEventArgs);

    return hr;
}

HRESULT CRangeBase::GetEffectiveMaximum(FLOAT* pMaximum)
{
    HRESULT hr = S_OK;
    CFloatValue* pEffectiveValue = NULL;

    IFCPTR(pMaximum);

    IFC(m_Maximum.GetTypedEffectiveValue(&pEffectiveValue));

    *pMaximum = pEffectiveValue->GetValue();

Cleanup:
    ReleaseObject(pEffectiveValue);

    return hr;
}

HRESULT CRangeBase::GetEffectiveMinimum(FLOAT* pMinimum)
{
    HRESULT hr = S_OK;
    CFloatValue* pEffectiveValue = NULL;

    IFCPTR(pMinimum);

    IFC(m_Minimum.GetTypedEffectiveValue(&pEffectiveValue));

    *pMinimum = pEffectiveValue->GetValue();

Cleanup:
    ReleaseObject(pEffectiveValue);

    return hr;
}

HRESULT CRangeBase::GetEffectiveRangeValue(FLOAT* pValue)
{
    HRESULT hr = S_OK;
    CFloatValue* pEffectiveValue = NULL;

    IFCPTR(pValue);

    IFC(m_Value.GetTypedEffectiveValue(&pEffectiveValue));

    *pValue = pEffectiveValue->GetValue();

Cleanup:
    ReleaseObject(pEffectiveValue);

    return hr;
}