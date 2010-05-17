#include "FrameworkElement.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"
#include "BasicTypes.h"

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
                                         m_ResolvedStyle(NULL)
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

    IFC(m_Resources.SetLocalValue(pResources, GetProviders()));

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

    pNamescope = GetNamescope();
    IFCPTR(pNamescope);

    IFC(GetEffectiveName(&pName));

    if(pName)
    {
        IFC(pNamescope->RegisterName(pName->GetValue(), this));

        IFC(pName->Clone(&m_RegisteredName));
    }

    {
        CUIAttachContext ChildContext(this, GetTemplateParentForChildren());

        for(UINT i = 0; i < m_Children->GetCount(); i++)
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

        for(UINT i = 0; i < m_Children->GetCount(); i++)
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

    IFC(m_Name.GetTypedEffectiveValue(GetProviders(), ppName));

Cleanup:
    return hr;
}

HRESULT CFrameworkElement::GetEffectiveStyle(CStyle** ppStyle)
{
    HRESULT hr = S_OK;

    IFCPTR(ppStyle);

    IFC(m_Style.GetTypedEffectiveValue(GetProviders(), ppStyle));

Cleanup:
    return hr;
}

HRESULT CFrameworkElement::GetEffectiveResources(CResourceDictionary** ppResources)
{
    HRESULT hr = S_OK;

    IFCPTR(ppResources);

    IFC(m_Resources.GetTypedEffectiveValue(GetProviders(), ppResources));

Cleanup:
    return hr;
}

CNamescope* CFrameworkElement::GetNamescope()
{
    CNamescope* pNamescope = NULL;
    CUIElement* pParent = NULL;

    pParent = GetParent();

    if(pParent != NULL && pParent->IsTypeOf(TypeIndex::FrameworkElement))
    {
        CFrameworkElement* pParentFrameworkElement = (CFrameworkElement*)pParent;

        pNamescope = pParentFrameworkElement->GetNamescope();
    }

    return pNamescope;
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

VOID CFrameworkElement::OnChildAdded(CUIElement* pElement)
{
    HRESULT hr = S_OK;

    IFCPTR(pElement);

    IFC(AddChildVisual(pElement));

    if(IsAttached())
    {
        CUIAttachContext ChildContext(this, GetTemplateParentForChildren());

        IFC(pElement->OnAttach(ChildContext));
    }

Cleanup:
    ;
}

VOID CFrameworkElement::OnChildRemoved(CUIElement* pElement)
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
    CObjectWithType* pConvertedType = NULL;
    CLayeredValue* pLayeredValue = NULL;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    IFC(GetLayeredValue(pProperty, &pLayeredValue));

    IFC(pLayeredValue->SetStyleValue(pValue, GetProviders()));

    //TODO: Resolve this for layered properties.
    IFC(RaisePropertyChanged(pProperty));

Cleanup:
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

            IFC(pParentFrameworkElement->FindResource(pKey, ppObject));
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
    CDelegatingPropertyInformation* pDelegatingProperyInformation = NULL;

    IFCPTR(ppInformation);

    CStaticProperty* Properties[] = 
    {
        &NameProperty,
        &ResourcesProperty,
        &StyleProperty
    };

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation))
    IFC(CUIElement::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingProperyInformation));

    *ppInformation = pDelegatingProperyInformation;
    pDelegatingProperyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingProperyInformation);

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

    for(UINT i = 0; i < GetCount(); i++)
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