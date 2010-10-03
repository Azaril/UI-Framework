#include "FrameworkElement.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"
#include "BasicTypes.h"
#include "BindingManager.h"

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( Name );
DEFINE_GET_DEFAULT_NULL( Resources );
DEFINE_GET_DEFAULT_NULL( Style );

//
// Properties
//
CStaticProperty CFrameworkElement::NameProperty( L"Name", TypeIndex::String, StaticPropertyFlags::None, &GET_DEFAULT( Name ), &INSTANCE_CHANGE_CALLBACK( CFrameworkElement, OnNameChanged ) );
CStaticProperty CFrameworkElement::ResourcesProperty( L"Resources", TypeIndex::Object, StaticPropertyFlags::Dictionary | StaticPropertyFlags::ReadOnly, &GET_DEFAULT( Resources ) );
CStaticProperty CFrameworkElement::StyleProperty( L"Style", TypeIndex::Style, StaticPropertyFlags::None, &GET_DEFAULT( Style ), &INSTANCE_CHANGE_CALLBACK( CFrameworkElement, OnStyleChanged ) );

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CFrameworkElement, OnNameChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CFrameworkElement, OnResourcesChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CFrameworkElement, OnStyleChanged );

CFrameworkElement::CFrameworkElement() : m_ChildrenSubscriber(*this),
                                         m_StyleCallback(*this),
                                         m_StyleDirty(FALSE),
                                         m_Name(this, &CFrameworkElement::NameProperty),
                                         m_Resources(this, &CFrameworkElement::ResourcesProperty),
                                         m_Style(this, &CFrameworkElement::StyleProperty),
                                         m_Children(NULL),
                                         m_RegisteredName(NULL),
                                         m_ResolvedStyle(NULL),
                                         m_AutomaticNamescopeParticipation(TRUE)
{
}

CFrameworkElement::~CFrameworkElement()
{
    ReleaseObject(m_Children);
    ReleaseObject(m_RegisteredName);
    ReleaseObject(m_ResolvedStyle);
}

HRESULT CFrameworkElement::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;
    CResourceDictionary* pResources = NULL;

    IFC(CUIElement::Initialize(pProviders));

    IFC(CResourceDictionary::Create(&pResources));

    IFC(m_Resources.SetLocalValue(pResources));

    IFC(CUIElementCollection::Create(&m_Children));

    IFC(m_Children->AddSubscriber(&m_ChildrenSubscriber));

Cleanup:
    ReleaseObject(pResources);

    return hr;
}

HRESULT CFrameworkElement::OnAttach(CUIAttachContext& Context)
{
    HRESULT hr = S_OK;
    CNamescope* pNamescope = NULL;
    CStringValue* pName = NULL;

    IFC(CUIElement::OnAttach(Context));

    if(m_AutomaticNamescopeParticipation)
    {
        pNamescope = GetNamescope();
        IFCPTR(pNamescope);

        IFC(RegisterInNamescope(pNamescope, &m_RegisteredName));
    }

    {
        CUIAttachContext ChildContext(GetStaticTreeData(), this, GetTemplateParentForChildren(), GetNamescopeForChildren());

        for(UINT32 i = 0; i < m_Children->GetCount(); i++)
        {
            CUIElement* pElement = m_Children->GetAtIndex(i);

            if(!pElement->IsAttached())
            {
                IFC(pElement->OnAttach(ChildContext));
            }
        }
    }

    m_StyleDirty = TRUE;

    IFC(EnsureStyle());

Cleanup:
    ReleaseObject(pName);

    return hr;
}

HRESULT CFrameworkElement::OnDetach(CUIDetachContext& Context)
{
    HRESULT hr = S_OK;
    CNamescope* pNamescope = NULL;

    pNamescope = GetNamescope();
    IFCPTR(pNamescope);

    if(m_RegisteredName)
    {
        IFC(pNamescope->UnregisterName(m_RegisteredName->GetValue(), this));
    }

    {
        CUIDetachContext ChildContext(this);

        for(UINT32 i = 0; i < m_Children->GetCount(); i++)
        {
            CUIElement* pElement = m_Children->GetAtIndex(i);

            IFC(pElement->OnDetach(ChildContext));
        }
    }

    IFC(CUIElement::OnDetach(Context));

Cleanup:
    ReleaseObject(m_RegisteredName);

    return hr;
}

HRESULT CFrameworkElement::GetEffectiveName(CStringValue** ppName)
{
    HRESULT hr = S_OK;

    IFCPTR(ppName);

    IFC(m_Name.GetTypedEffectiveValue(ppName));

Cleanup:
    return hr;
}

HRESULT CFrameworkElement::GetEffectiveStyle(CStyle** ppStyle)
{
    HRESULT hr = S_OK;

    IFCPTR(ppStyle);

    IFC(m_Style.GetTypedEffectiveValue(ppStyle));

Cleanup:
    return hr;
}

HRESULT CFrameworkElement::GetEffectiveResources(CResourceDictionary** ppResources)
{
    HRESULT hr = S_OK;

    IFCPTR(ppResources);

    IFC(m_Resources.GetTypedEffectiveValue(ppResources));

Cleanup:
    return hr;
}

HRESULT CFrameworkElement::FindName(const WCHAR* pName, CObjectWithType** ppObject)
{
    HRESULT hr = S_OK;
    CNamescope* pNamescope = NULL;

    IFCPTR(pName);
    IFCPTR(ppObject);

    pNamescope = GetNamescope();
    IFCPTR(pNamescope);

    IFC(pNamescope->FindName(pName, ppObject));

Cleanup:
    return hr;
}

CUIElement* CFrameworkElement::GetTemplateParentForChildren()
{
    return GetTemplateParent();
}

CNamescope* CFrameworkElement::GetNamescopeForChildren()
{
    return GetNamescope();
}

HRESULT CFrameworkElement::AddLogicalChild(CUIElement* pElement)
{
    HRESULT hr = S_OK;

    IFCPTR(pElement);

    IFC(m_Children->AddObject(pElement));

Cleanup:
    return hr;
}

HRESULT CFrameworkElement::RemoveLogicalChild(CUIElement* pElement)
{
    HRESULT hr = S_OK;

    IFCPTR(pElement);

    IFC(m_Children->RemoveObject(pElement));

Cleanup:
    return hr;
}

CUIElementCollection* CFrameworkElement::GetChildCollection()
{
    return m_Children;
}

void CFrameworkElement::OnChildAdded(CUIElement* pElement)
{
    HRESULT hr = S_OK;

    IFCPTR(pElement);

    IFC(AddChildVisual(pElement));

    if(IsAttached())
    {
        CUIAttachContext ChildContext(GetStaticTreeData(), this, GetTemplateParentForChildren(), GetNamescopeForChildren());

        IFC(pElement->OnAttach(ChildContext));
    }

Cleanup:
    ;
}

void CFrameworkElement::OnChildRemoved(CUIElement* pElement)
{
    HRESULT hr = S_OK;

    IFCPTR(pElement);

    if(IsAttached())
    {
        CUIDetachContext ChildContext(this);

        IFC(pElement->OnDetach(ChildContext));
    }

    IFC(RemoveChildVisual(pElement));

Cleanup:
    ;
}

HRESULT CFrameworkElement::SetName(const WCHAR* pName)
{
    HRESULT hr = S_OK;
    CStringValue* pNameValue = NULL;

    IFCPTR(pName);

    IFC(CStringValue::Create(pName, &pNameValue));

    IFC(SetValue(&CFrameworkElement::NameProperty, pNameValue));

Cleanup:
    ReleaseObject(pNameValue);

    return hr;
}

HRESULT CFrameworkElement::OnNameChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;
    CNamescope* pNamescope = NULL;

    pNamescope = GetNamescope();

    if(m_RegisteredName)
    {
        if(pNamescope)
        {
            IFC(pNamescope->UnregisterName(m_RegisteredName->GetValue(), this));
        }

        ReleaseObject(m_RegisteredName);
    }

    if(pNamescope && pNewValue)
    {
        CStringValue* pNewName = NULL;

        IFC(CastType(pNewValue, &pNewName));

        if(pNewValue)
        {
            IFC(pNamescope->RegisterName(pNewName->GetValue(), this));
        }

        m_RegisteredName = pNewName;
        AddRefObject(m_RegisteredName);
    }

Cleanup:
    return hr;
}

HRESULT CFrameworkElement::OnResourcesChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    //TODO: Handle resource being replaced? Currently the property is marked as read only so this shouldn't happen.

    return hr;
}

HRESULT CFrameworkElement::OnStyleChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    m_StyleDirty = TRUE;

    IFC(RevokeStyle());

    IFC(EnsureStyle());

Cleanup:
    return hr;
}

HRESULT CFrameworkElement::EnsureStyle()
{
    HRESULT hr = S_OK;
    CStyle* pStyle = NULL;
    CTypeValue* pTypeValue = NULL;

    if(m_StyleDirty)
    {
        if(IsAttached())
        {
            IFC(RevokeStyle());

            IFC(GetEffectiveStyle(&pStyle));

            if(pStyle == NULL)
            {
                //Find the default template for this type.
                IFC(CTypeValue::Create(GetType(), &pTypeValue));

                if(FAILED(FindResource(pTypeValue, &pStyle)))
                {
                    pStyle = NULL;
                }
            }

            if(pStyle)
            {
                IFC(ApplyStyle(pStyle));
            }

            m_StyleDirty = FALSE;
        }
    }

Cleanup:
    ReleaseObject(pStyle);
    ReleaseObject(pTypeValue);

    return hr;
}

HRESULT CFrameworkElement::RevokeStyle()
{
    HRESULT hr = S_OK;

    //TODO: Call revoke on the style or clear properties style values.

    ReleaseObject(m_ResolvedStyle);

    return hr;
}

HRESULT CFrameworkElement::ApplyStyle(CStyle* pStyle)
{
    HRESULT hr = S_OK;

    IFCPTR(pStyle);

    IFCEXPECT(m_ResolvedStyle == NULL);

    IFC(pStyle->ResolveStyle(this, &m_StyleCallback, &m_ResolvedStyle));

Cleanup:
    return hr;
}

HRESULT CFrameworkElement::SetStyleValue(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;
    CLayeredValue* pLayeredValue = NULL;
    CBindingBase* pBinding = NULL;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    if(pValue->IsTypeOf(TypeIndex::BindingBase))
    {
        IFC(CastType(pValue, &pBinding));

        IFC(SetStyleBinding(pProperty, pBinding));
    }
    else
    {
        IFC(SetStyleValueInternal(pProperty, pValue));
    }

Cleanup:
    return hr;
}

HRESULT CFrameworkElement::SetStyleValueInternal(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;
    CLayeredValue* pLayeredValue = NULL;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    IFC(GetLayeredValue(pProperty, &pLayeredValue));

    IFC(pLayeredValue->SetStyleValue(pValue));

Cleanup:
    return hr;
}

HRESULT CFrameworkElement::SetStyleBinding(CProperty* pProperty, CBindingBase* pBinding)
{
    HRESULT hr = S_OK;

    IFC(GetBindingManager()->SetBinding(this, pProperty, pBinding, &CFrameworkElement::SetStyleBindingValue));

Cleanup:
    return hr;
}

HRESULT CFrameworkElement::SetStyleBindingValue(CPropertyObject* pTarget, CProperty* pTargetProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;
    CFrameworkElement* pElement = NULL;

    IFC(CastType(pTarget, &pElement));

    IFC(pElement->SetStyleValueInternal(pTargetProperty, pValue));

Cleanup:
    return hr;
}

VOID CFrameworkElement::SetAutomaticNamescopeParticipation(BOOL Participate)
{
    m_AutomaticNamescopeParticipation = Participate;
}

HRESULT CFrameworkElement::RegisterInNamescope(CNamescope* pNamescope, CStringValue** ppRegisteredName)
{
    HRESULT hr = S_OK;
    CStringValue* pName = NULL;

    IFCPTR(pNamescope);

    IFC(GetEffectiveName(&pName));

    if(pName)
    {
        IFC(pNamescope->RegisterName(pName->GetValue(), this));

        if(ppRegisteredName)
        {
            *ppRegisteredName = pName;
            pName = NULL;
        }
    }

Cleanup:
    ReleaseObject(pName);

    return hr;
}

HRESULT CFrameworkElement::FindResource(const WCHAR* pResourceName, CObjectWithType** ppObject)
{
    HRESULT hr = S_OK;
    CStringValue* pStringValue = NULL;

    IFCPTR(pResourceName);
    IFCPTR(ppObject);

    IFC(CStringValue::Create(pResourceName, &pStringValue));

    IFC(FindResource(pStringValue, ppObject));

Cleanup:
    ReleaseObject(pStringValue);

    return hr;
}

HRESULT CFrameworkElement::FindResource(CObjectWithType* pKey, CObjectWithType** ppObject)
{
    HRESULT hr = S_OK;
    CResourceDictionary* pResources = NULL;

    IFC(GetEffectiveResources(&pResources));

    if(pResources)
    {
        hr = pResources->GetObject(pKey, ppObject);
    }

    if(pResources == NULL || FAILED(hr))
    {
        CUIElement* pParent = GetParent();

        if(pParent != NULL && pParent->IsTypeOf(TypeIndex::FrameworkElement))
        {
            CFrameworkElement* pParentFrameworkElement = (CFrameworkElement*)pParent;

            IFC_NOTRACE(pParentFrameworkElement->FindResource(pKey, ppObject));
        }
    }

Cleanup:
    ReleaseObject(pResources);

    return hr;
}

HRESULT CFrameworkElement::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingPropertyInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &NameProperty,
        &ResourcesProperty,
        &StyleProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation))
    IFC(CUIElement::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingPropertyInformation));

    *ppInformation = pDelegatingPropertyInformation;
    pDelegatingPropertyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingPropertyInformation);

    return hr;
}

HRESULT CFrameworkElement::GetLayeredValue(CProperty* pProperty, CLayeredValue** ppLayeredValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    //TODO: Make this a lookup table rather than requiring a comparison per property.
    if(pProperty == &CFrameworkElement::NameProperty)
    {
        *ppLayeredValue = &m_Name;
    }
    else if(pProperty == &CFrameworkElement::ResourcesProperty)
    {
        *ppLayeredValue = &m_Resources;
    }
    else if(pProperty == &CFrameworkElement::StyleProperty)
    {
        *ppLayeredValue = &m_Style;
    }
    else
    {
        hr = CUIElement::GetLayeredValue(pProperty, ppLayeredValue);
    }

Cleanup:
    return hr;
}

HRESULT CUIElementCollection::AddObject(CObjectWithType* pObject)
{
    HRESULT hr  = S_OK;

    IFCPTR(pObject);

    for(UINT32 i = 0; i < GetCount(); i++)
    {
        CUIElement* pChildElement = GetAtIndex(i);

        if(pChildElement == pObject)
        {
            IFC(E_FAIL);
        }
    }

    IFC(CCollection< CUIElement >::AddObject(pObject));

Cleanup:
    return hr;
}

//
// CFrameworkElement
//
extern "C" __declspec(dllexport)
TypeIndex::Value CFrameworkElement_TypeIndex()
{
    return TypeIndex::FrameworkElement;
}


extern "C" __declspec(dllexport)
CUIElement* CFrameworkElement_CastTo_CUIElement(CFrameworkElement* pElement)
{
    return pElement;
}

extern "C" __declspec(dllexport)
CFrameworkElement* CObjectWithType_CastTo_CFrameworkElement(CObjectWithType* pObject)
{
    return (pObject->IsTypeOf(TypeIndex::FrameworkElement)) ? (CFrameworkElement*)pObject : NULL;
}