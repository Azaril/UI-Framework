#include "DynamicResource.h"
#include "FrameworkElement.h"
#include "DelegatingPropertyInformation.h"
#include "StaticPropertyInformation.h"

//
// Properties
//
CStaticProperty CDynamicResource::ResourceKeyProperty( L"ResourceKey", TypeIndex::Object, StaticPropertyFlags::None );

CDynamicResource::CDynamicResource() : m_ResourceKey(NULL)
{
}

CDynamicResource::~CDynamicResource()
{
    m_TargetAttachedConnection.disconnect();
    m_TargetDetachedConnection.disconnect();

    ReleaseObject(m_ResourceKey);
}

HRESULT CDynamicResource::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFC(CBindingBase::Initialize(pProviders));

Cleanup:
    return hr;
}

HRESULT CDynamicResource::GetBoundValue(CObjectWithType** ppValue)
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

HRESULT CDynamicResource::CreatePropertyInformation(CPropertyInformation **ppInformation)
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

HRESULT CDynamicResource::SetValueInternal(CProperty* pProperty, CObjectWithType* pValue)
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

HRESULT CDynamicResource::GetValueInternal(CProperty* pProperty, CObjectWithType** ppValue)
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

HRESULT CDynamicResource::SetTarget(CPropertyObject* pTarget, CProperty* pTargetProperty)
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

HRESULT CDynamicResource::ClearTarget()
{
    HRESULT hr = S_OK;

    IFC(CBindingBase::ClearTarget());

    m_TargetAttachedConnection.disconnect();
    m_TargetDetachedConnection.disconnect();

Cleanup:
    return hr;
}

void CDynamicResource::OnTargetAttached(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

    IFC(InvalidateBinding());

Cleanup:
    ;
}

void CDynamicResource::OnTargetDetached(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

    IFC(InvalidateBinding());

Cleanup:
    ;
}