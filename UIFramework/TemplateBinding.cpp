#include "TemplateBinding.h"
#include "UIElement.h"
#include "DelegatingPropertyInformation.h"

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( Property );

//
// Properties
//
CStaticProperty CTemplateBinding::PropertyProperty(L"Property", TypeIndex::String, StaticPropertyFlags::None, &GET_DEFAULT( Property ));

CTemplateBinding::CTemplateBinding(
    ) 
    : m_Property(NULL)
{
}

CTemplateBinding::~CTemplateBinding(
    )
{
    m_TargetAttachedConnection.disconnect();
    m_TargetDetachedConnection.disconnect();

    ReleaseObject(m_Property);
}

__checkReturn HRESULT 
CTemplateBinding::Initialize(
    __in CProviders* pProviders
    )
{
    HRESULT hr = S_OK;

    IFC(CSourcedBinding::Initialize(pProviders));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CTemplateBinding::CreatePropertyInformation(
    __deref_out CPropertyInformation **ppInformation
    )
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingPropertyInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &PropertyProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));
    IFC(CSourcedBinding::CreatePropertyInformation(&pBaseInformation));
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
CTemplateBinding::SetValueInternal(
    __in CProperty* pProperty, 
    __in CObjectWithType* pValue
    )
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
        IFC(CSourcedBinding::SetValueInternal(pProperty, pValue));
    }

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CTemplateBinding::GetValue(
    __in CProperty* pProperty, 
    __deref_out CObjectWithType** ppValue
    )
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
        IFC(CSourcedBinding::GetValue(pProperty, ppValue));
    }

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CTemplateBinding::SetTarget(
    __in CPropertyObject* pTarget, 
    __in CProperty* pTargetProperty
    )
{
    HRESULT hr = S_OK;
    CUIElement* pTargetElement = NULL;

    IFCPTR(pTarget);

    //TODO: Fix this, as currently you can only bind a template binding to a UI element.
    IFCEXPECT(pTarget->IsTypeOf(TypeIndex::UIElement));

    IFC(CSourcedBinding::SetTarget(pTarget, pTargetProperty));

    pTargetElement = (CUIElement*)pTarget;

    IFC(pTargetElement->AddHandler(&CUIElement::AttachedEvent, bind(&CTemplateBinding::OnTargetAttached, this, _1, _2), &m_TargetAttachedConnection));
    IFC(pTargetElement->AddHandler(&CUIElement::DetachedEvent, bind(&CTemplateBinding::OnTargetDetached, this, _1, _2), &m_TargetDetachedConnection));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT
CTemplateBinding::ClearTarget(
    )
{
    HRESULT hr = S_OK;

    IFC(CSourcedBinding::ClearTarget());

    m_TargetAttachedConnection.disconnect();
    m_TargetDetachedConnection.disconnect();

Cleanup:
    return hr;
}

void 
CTemplateBinding::OnTargetAttached(
    __in CObjectWithType* pSender,
    __in CRoutedEventArgs* pRoutedEventArgs
    )
{
    HRESULT hr = S_OK;
    CUIElement* pTargetElement = NULL;
    CUIElement* pTemplateParent = NULL;
    CClassResolver* pClassResolver = NULL;
    CProperty* pResolvedProperty = NULL;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

    IFC(CastType(GetTarget(), &pTargetElement));

    IFCPTR(pTargetElement);

    pTemplateParent = pTargetElement->GetTemplateParent();

    IFCPTR(m_Property);
    IFCPTR(pTemplateParent);

    pClassResolver = m_Providers->GetClassResolver();
    IFCPTR(pClassResolver);

    IFC(pClassResolver->ResolveProperty(m_Property->GetValue(), pTemplateParent->GetType(), &pResolvedProperty));

    IFC(SetSource(pTemplateParent, pResolvedProperty));

Cleanup:
    ReleaseObject(pResolvedProperty);
}

void 
CTemplateBinding::OnTargetDetached(
    __in CObjectWithType* pSender, 
    __in CRoutedEventArgs* pRoutedEventArgs
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

    IFC(ClearSource());

    IFC(InvalidateBinding());

Cleanup:
    ;
}