#include "DynamicResource.h"
#include "FrameworkElement.h"
#include "DelegatingPropertyInformation.h"
#include "StaticPropertyInformation.h"

//
// Properties
//
namespace DynamicResourceProperties
{
    enum Value
    {
        ResourceKey
    };
}

//
// Properties
//
CStaticProperty CDynamicResource::ResourceKeyProperty(TypeIndex::DynamicResource, DynamicResourceProperties::ResourceKey, L"ResourceKey", TypeIndex::Object, StaticPropertyFlags::None);

CDynamicResource::CDynamicResource(
    )
{
}

CDynamicResource::~CDynamicResource(
    )
{
    m_TargetAttachedConnection.disconnect();
    m_TargetDetachedConnection.disconnect();
}

__checkReturn HRESULT 
CDynamicResource::Initialize(
    __in CProviders* pProviders
    )
{
    HRESULT hr = S_OK;

    IFC(CBindingBase::Initialize(pProviders));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CDynamicResource::GetBoundValue(
    __deref_out_opt CObjectWithType** ppValue
    )
{
    HRESULT hr = S_OK;
    CPropertyObject* pTarget = NULL;
    CObjectWithType* pResourceKey = NULL;

    IFCPTR(ppValue);

    IFC(GetEffectiveValue(&ResourceKeyProperty, &pResourceKey));

    IFCPTR(pResourceKey);

    pTarget = GetTarget();
    IFCPTR(pTarget);

    if(pTarget->IsTypeOf(TypeIndex::FrameworkElement))
    {
        CFrameworkElement* pElement = (CFrameworkElement*)pTarget;

        if(FAILED(pElement->FindResource(pResourceKey, ppValue)))
        {
            *ppValue = NULL;
        }
    }
    else
    {
        IFC(E_FAIL);
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CDynamicResource::CreatePropertyInformation(
    __deref_out CPropertyInformation** ppInformation
    )
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingPropertyInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &ResourceKeyProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));
    IFC(CBindingBase::CreatePropertyInformation(&pBaseInformation));
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
CDynamicResource::GetLayeredValue(
    __in CProperty* pProperty,
    __deref_out CLayeredValue** ppLayeredValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    if (pProperty->GetOwningType() == TypeIndex::DynamicResource)
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        switch(pStaticProperty->GetLocalIndex())
        {
            case DynamicResourceProperties::ResourceKey:
                {
                    *ppLayeredValue = &m_ResourceKey;
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
        IFC_NOTRACE(CBindingBase::GetLayeredValue(pProperty, ppLayeredValue));
    }

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CDynamicResource::SetTarget(
    __in CPropertyObject* pTarget, 
    __in CProperty* pTargetProperty
    )
{
    HRESULT hr = S_OK;
    CUIElement* pTargetElement = NULL;

    IFCPTR(pTarget);

    //TODO: Fix this, as currently you can only bind a template binding to a UI element.
    IFCEXPECT(pTarget->IsTypeOf(TypeIndex::UIElement));

    IFC(CBindingBase::SetTarget(pTarget, pTargetProperty));

    pTargetElement = (CUIElement*)pTarget;

    IFC(pTargetElement->AddHandler(&CUIElement::AttachedEvent, bind(&CDynamicResource::OnTargetAttached, this, _1, _2), &m_TargetAttachedConnection));
    IFC(pTargetElement->AddHandler(&CUIElement::DetachedEvent, bind(&CDynamicResource::OnTargetDetached, this, _1, _2), &m_TargetDetachedConnection));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CDynamicResource::ClearTarget(
    )
{
    HRESULT hr = S_OK;

    IFC(CBindingBase::ClearTarget());

    m_TargetAttachedConnection.disconnect();
    m_TargetDetachedConnection.disconnect();

Cleanup:
    return hr;
}

void 
CDynamicResource::OnTargetAttached(
    __in CObjectWithType* pSender,
    __in CRoutedEventArgs* pRoutedEventArgs
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

    IFC(InvalidateBinding());

Cleanup:
    ;
}

void 
CDynamicResource::OnTargetDetached(
    __in CObjectWithType* pSender,
    __in CRoutedEventArgs* pRoutedEventArgs
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

    IFC(InvalidateBinding());

Cleanup:
    ;
}