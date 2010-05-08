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
    ReleaseObject(m_ResourceKey);
}

HRESULT CDynamicResource::Initialize()
{
    HRESULT hr = S_OK;

Cleanup:
    return hr;
}

HRESULT CDynamicResource::GetBoundValue(CObjectWithType* pTarget, CProperty* pTargetProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pTarget);
    IFCPTR(pTargetProperty);
    IFCPTR(ppValue);

    IFCPTR(m_ResourceKey);

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
    CDelegatingPropertyInformation* pDelegatingProperyInformation = NULL;

    IFCPTR(ppInformation);

    CStaticProperty* Properties[] = 
    {
        &ResourceKeyProperty
    };

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));
    IFC(CBinding::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingProperyInformation));

    *ppInformation = pDelegatingProperyInformation;
    pDelegatingProperyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingProperyInformation);

    return hr;
}

HRESULT CDynamicResource::SetValue(CProperty* pProperty, CObjectWithType* pValue)
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
        IFC(CBinding::SetValue(pProperty, pValue));
    }

Cleanup:
    return hr;
}

HRESULT CDynamicResource::GetValue(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(pProperty  == &CDynamicResource::ResourceKeyProperty)
    {
        *ppValue = m_ResourceKey;
        AddRefObject(m_ResourceKey);
    }
    else
    {
        IFC(CBinding::GetValue(pProperty, ppValue));
    }

Cleanup:
    return hr;
}