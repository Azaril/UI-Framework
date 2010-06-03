#include "TemplateBinding.h"
#include "UIElement.h"

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( Property );

//
// Properties
//
CStaticProperty CTemplateBinding::PropertyProperty( L"Property", TypeIndex::String, StaticPropertyFlags::None, &GET_DEFAULT( Property ) );

CTemplateBinding::CTemplateBinding() : m_Property(NULL),
                                       m_ResolvedProperty(NULL)
{
}

CTemplateBinding::~CTemplateBinding()
{
    m_PropertyChangedConnection.disconnect();

    m_TargetAttachedConnection.disconnect();
    m_TargetDetachedConnection.disconnect();

    ReleaseObject(m_Property);
    ReleaseObject(m_ResolvedProperty);
}

HRESULT CTemplateBinding::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFC(CBinding::Initialize(pProviders));

Cleanup:
    return hr;
}

HRESULT CTemplateBinding::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &PropertyProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));

    *ppInformation = pStaticInformation;
    pStaticInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);

    return hr;
}

HRESULT CTemplateBinding::SetValueInternal(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    if(pProperty == &CTemplateBinding::PropertyProperty)
    {
        IFCEXPECT(pValue->IsTypeOf(TypeIndex::String));

        ReleaseObject(m_Property);

        m_Property = (CStringValue*)pValue;

        AddRefObject(m_Property);
    }
    else
    {
        IFC(CPropertyObject::SetValueInternal(pProperty, pValue));
    }

Cleanup:
    return hr;
}

HRESULT CTemplateBinding::GetValue(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(pProperty == &CTemplateBinding::PropertyProperty)
    {
        *ppValue = m_Property;
        AddRefObject(m_Property);
    }
    else
    {
        IFC(CPropertyObject::GetValue(pProperty, ppValue));
    }

Cleanup:
    return hr;
}

HRESULT CTemplateBinding::GetBoundValue(CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;
    CUIElement* pTargetElement = NULL;
    CUIElement* pTemplateParent = NULL;
    CProperty* pSourceProperty = NULL;
    CClassResolver* pClassResolver = NULL;
    CObjectWithType* pEffectiveValue = NULL;

    IFCPTR(ppValue);

    IFCPTR(m_Target);

    IFC(CastType(m_Target, &pTargetElement));

    pTemplateParent = pTargetElement->GetTemplateParent();

    IFCPTR(m_Property);
    IFCPTR(pTemplateParent);

    pClassResolver = m_Providers->GetClassResolver();
    IFCPTR(pClassResolver);

    IFC(pClassResolver->ResolveProperty(m_Property->GetValue(), pTemplateParent->GetType(), &pSourceProperty));

    IFC(pTemplateParent->GetEffectiveValue(pSourceProperty, &pEffectiveValue));

    IFCPTR(pEffectiveValue);

    *ppValue = pEffectiveValue;
    pEffectiveValue = NULL;
    
Cleanup:
    ReleaseObject(pSourceProperty);
    ReleaseObject(pEffectiveValue);

    return hr;
}

HRESULT CTemplateBinding::SetTarget(CPropertyObject* pTarget, CProperty* pTargetProperty)
{
    HRESULT hr = S_OK;
    CUIElement* pTargetElement = NULL;

    IFCPTR(pTarget);

    //TODO: Fix this, as currently you can only bind a template binding to a UI element.
    IFCEXPECT(pTarget->IsTypeOf(TypeIndex::UIElement));

    IFC(CBinding::SetTarget(pTarget, pTargetProperty));

    pTargetElement = (CUIElement*)pTarget;

    IFC(pTargetElement->AddHandler(&CUIElement::AttachedEvent, bind(&CTemplateBinding::OnTargetAttached, this, _1, _2), &m_TargetAttachedConnection));
    IFC(pTargetElement->AddHandler(&CUIElement::DetachedEvent, bind(&CTemplateBinding::OnTargetDetached, this, _1, _2), &m_TargetDetachedConnection));

Cleanup:
    return hr;
}

HRESULT CTemplateBinding::ClearTarget()
{
    HRESULT hr = S_OK;

    ReleaseObject(m_ResolvedProperty);

    IFC(CBinding::ClearTarget());

    m_TargetAttachedConnection.disconnect();
    m_TargetDetachedConnection.disconnect();

Cleanup:
    return hr;
}

void CTemplateBinding::OnTargetAttached(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;
    CUIElement* pTargetElement = NULL;
    CUIElement* pTemplateParent = NULL;
    CClassResolver* pClassResolver = NULL;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

    //TODO: Fix this, as currently you can only bind a template binding to a UI element.
    IFC(CastType(m_Target, &pTargetElement));

    pTemplateParent = pTargetElement->GetTemplateParent();

    IFCPTR(m_Property);
    IFCPTR(pTemplateParent);

    pClassResolver = m_Providers->GetClassResolver();
    IFCPTR(pClassResolver);

    IFC(pClassResolver->ResolveProperty(m_Property->GetValue(), pTemplateParent->GetType(), &m_ResolvedProperty));

    IFC(pTemplateParent->AddPropertyChangeListener(bind(&CTemplateBinding::OnSourcePropertyChanged, this, _1, _2), &m_PropertyChangedConnection));

    IFC(InvalidateBinding());

Cleanup:
    ;
}

void CTemplateBinding::OnTargetDetached(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

    m_PropertyChangedConnection.disconnect();

    IFC(InvalidateBinding());

Cleanup:
    ;
}

void CTemplateBinding::OnSourcePropertyChanged(CPropertyObject* pObject, CProperty* pProperty)
{
    HRESULT hr = S_OK;

    IFCPTR(pObject);
    IFCPTR(pProperty);

    if(pProperty == m_ResolvedProperty)
    {
        IFC(InvalidateBinding());
    }

Cleanup:
    ;
}