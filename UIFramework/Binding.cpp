#include "Binding.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"

//
// Property Defaults
//
//DEFINE_GET_DEFAULT( BindingDirection, CBindingDirectionValue, BindingDirection::OneWay );

//
// Properties
//
//CStaticProperty CBinding::BindingDirectionProperty(L"BindingDirection", TypeIndex::BindingDirection, StaticPropertyFlags::None, &GET_DEFAULT( BindingDirection ));

CBinding::CBinding(
    )
    // : m_BindingDirection(BindingDirection::OneWay)
{
}

CBinding::~CBinding(
    )
{
}

__checkReturn HRESULT 
CBinding::Initialize(
    __in CProviders* pProviders
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProviders);

    IFC(CSourcedBinding::Initialize(pProviders));

Cleanup:
    return hr;
}

//BindingDirection::Value 
//CBinding::GetBindingDirection(
//    )
//{
//    return m_BindingDirection;
//}

//__checkReturn HRESULT
//CBinding::CreatePropertyInformation(
//    __deref_out CPropertyInformation** ppInformation
//  )
//{
//    HRESULT hr = S_OK;
//    CStaticPropertyInformation* pStaticInformation = NULL;
//    CPropertyInformation* pBaseInformation = NULL;
//    CDelegatingPropertyInformation* pDelegatingPropertyInformation = NULL;
//
//    CStaticProperty* Properties[] = 
//    {
//        &BindingDirectionProperty
//    };
//    
//    IFCPTR(ppInformation);
//
//    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));
//    IFC(CBindingBase::CreatePropertyInformation(&pBaseInformation));
//    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingPropertyInformation));
//
//    *ppInformation = pDelegatingPropertyInformation;
//    pDelegatingPropertyInformation = NULL;
//
//Cleanup:
//    ReleaseObject(pStaticInformation);
//    ReleaseObject(pBaseInformation);
//    ReleaseObject(pDelegatingPropertyInformation);
//
//    return hr;
//}

__override __checkReturn HRESULT 
CBinding::SetValueInternal(
    __in CProperty* pProperty, 
    __in CObjectWithType* pValue
    )
{
    HRESULT hr = S_OK;
    //CBindingDirectionValue* pBindingDirection = NULL;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    //if(pProperty == &CBinding::BindingDirectionProperty)
    //{
    //    IFCPTR(pValue);

    //    IFC(CastType(pValue, &pBindingDirection));

    //    m_BindingDirection = pBindingDirection->GetValue();
    //}
    //else
    //{
        IFC(CBindingBase::SetValueInternal(pProperty, pValue));
    //}

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CBinding::GetValueInternal(
    __in CProperty* pProperty, 
    __deref_out CObjectWithType** ppValue
    )
{
    HRESULT hr = S_OK;
    //CBindingDirectionValue* pBindingDirection = NULL;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    //if(pProperty == &CBinding::BindingDirectionProperty)
    //{
    //    IFC(CBindingDirectionValue::Create(m_BindingDirection, &pBindingDirection));

    //    *ppValue = pBindingDirection;
    //    pBindingDirection = NULL;
    //}
    //else
    //{
        IFC(CBindingBase::GetValue(pProperty, ppValue));
    //}

Cleanup:
    //ReleaseObject(pBindingDirection);

    return hr;
}