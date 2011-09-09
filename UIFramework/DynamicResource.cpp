#include "DynamicResource.h"
#include "FrameworkElement.h"
#include "DelegatingPropertyInformation.h"
#include "StaticPropertyInformation.h"

//
// Properties
//
CStaticProperty CDynamicResource::ResourceKeyProperty(L"ResourceKey", TypeIndex::Object, StaticPropertyFlags::None);

CDynamicResource::CDynamicResource(
    ) 
    : m_ResourceKey(NULL)
{
}

CDynamicResource::~CDynamicResource(
    )
{
    m_TargetAttachedConnection.disconnect();
    m_TargetDetachedConnection.disconnect();

    ReleaseObject(m_ResourceKey);
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

    IFCPTR(ppValue);

    IFCPTR(m_ResourceKey);

    pTarget = GetTarget();
    IFCPTR(pTarget);

    if(pTarget->IsTypeOf(TypeIndex::FrameworkElement))
    {
        CFrameworkElement* pElement = (CFrameworkElement*)pTarget;

        if(FAILED(pElement->FindResource(m_ResourceKey, ppValue)))
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
CDynamicResource::SetValueInternal(
    __in CProperty* pProperty,
    __in CObjectWithType* pValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    if(pProperty == &CDynamicResource::ResourceKeyProperty)
    {
        ReleaseObject(m_ResourceKey);
        m_ResourceKey = pValue;
        AddRefObject(m_ResourceKey);
    }
    else
    {
        IFC(CBindingBase::SetValueInternal(pProperty, pValue));
    }

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CDynamicResource::GetValueInternal(
    __in CProperty* pProperty, 
    __deref_out_opt CObjectWithType** ppValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(pProperty == &CDynamicResource::ResourceKeyProperty)
    {
        *ppValue = m_ResourceKey;
        AddRefObject(m_ResourceKey);
    }
    else
    {
        IFC(CBindingBase::GetValue(pProperty, ppValue));
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