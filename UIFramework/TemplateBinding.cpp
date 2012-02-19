#include "TemplateBinding.h"
#include "UIElement.h"
#include "DelegatingPropertyInformation.h"

//
// Properties
//
namespace TemplateBindingProperties
{
    enum Value
    {
        Property
    };
}

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( Property );

//
// Properties
//
CStaticProperty CTemplateBinding::PropertyProperty(TypeIndex::TemplateBinding, TemplateBindingProperties::Property, L"Property", TypeIndex::String, StaticPropertyFlags::None, &GET_DEFAULT( Property ));

CTemplateBinding::CTemplateBinding(
    )
{
}

CTemplateBinding::~CTemplateBinding(
    )
{
    m_TargetAttachedConnection.disconnect();
    m_TargetDetachedConnection.disconnect();
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
CTemplateBinding::GetLayeredValue(
    __in CProperty* pProperty,
    __deref_out CLayeredValue** ppLayeredValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    if (pProperty->GetOwningType() == TypeIndex::TemplateBinding)
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        switch(pStaticProperty->GetLocalIndex())
        {
            case TemplateBindingProperties::Property:
                {
                    *ppLayeredValue = &m_Property;
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
        IFC_NOTRACE(CSourcedBinding::GetLayeredValue(pProperty, ppLayeredValue));
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
    CPropertyObject* pTarget = NULL;
    CUIElement* pTargetElement = NULL;
    CUIElement* pTemplateParent = NULL;
    CClassResolver* pClassResolver = NULL;
    CProperty* pResolvedProperty = NULL;
    CStringValue* pProperty = NULL;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

    pTarget = GetTarget();
    IFCPTR(pTarget);

    IFC(CastType(pTarget, &pTargetElement));

    IFCPTR(pTargetElement);

    pTemplateParent = pTargetElement->GetTemplateParent();
    IFCPTR(pTemplateParent);

    pClassResolver = GetProviders()->GetClassResolver();
    IFCPTR(pClassResolver);

    IFC(GetTypedEffectiveValue(&PropertyProperty, &pProperty));

    IFCPTR(pProperty);

    IFC(pClassResolver->ResolveProperty(pProperty->GetValue(), pTemplateParent->GetType(), &pResolvedProperty));

    IFC(SetSource(pTemplateParent, pResolvedProperty));

Cleanup:
    ReleaseObject(pResolvedProperty);
    ReleaseObject(pProperty);
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