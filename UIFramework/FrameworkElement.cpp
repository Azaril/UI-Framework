#include "FrameworkElement.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"
#include "BasicTypes.h"

//
// Properties
//
CStaticProperty CFrameworkElement::NameProperty( L"Name", TypeIndex::String, StaticPropertyFlags::None );
CStaticProperty CFrameworkElement::ResourcesProperty( L"Resources", TypeIndex::Object, StaticPropertyFlags::Dictionary );
CStaticProperty CFrameworkElement::StyleProperty( L"Style", TypeIndex::Style, StaticPropertyFlags::None );

CFrameworkElement::CFrameworkElement() : m_Children(NULL),
                                         m_ChildrenSubscriber(*this),
                                         m_SetterResolvedCallback(*this),
                                         m_Resources(NULL),
                                         m_Style(NULL),
                                         m_StyleDirty(FALSE)
{
}

CFrameworkElement::~CFrameworkElement()
{
    ReleaseObject(m_Children);
    ReleaseObject(m_Resources);
    ReleaseObject(m_Style);
}

HRESULT CFrameworkElement::Initialize()
{
    HRESULT hr = S_OK;

    IFC(CUIElement::Initialize());

    IFC(CResourceDictionary::Create(&m_Resources));

    IFC(CUIElementCollection::Create(&m_Children));

    IFC(m_Children->AddSubscriber(&m_ChildrenSubscriber));

Cleanup:
    return hr;
}

HRESULT CFrameworkElement::OnAttach(CUIAttachContext& Context)
{
    HRESULT hr = S_OK;
    CNamescope* pNamescope = NULL;

    IFC(CUIElement::OnAttach(Context));

    pNamescope = GetNamescope();
    IFCPTR(pNamescope);

    if(!m_Name.empty())
    {
        IFC(pNamescope->RegisterName(m_Name.c_str(), this));
    }

    IFC(EnsureStyle());

    {
        CUIAttachContext ChildContext(this, GetProviders());

        for(UINT i = 0; i < m_Children->GetCount(); i++)
        {
            CUIElement* pElement = m_Children->GetAtIndex(i);

            IFC(pElement->OnAttach(ChildContext));
        }
    }

Cleanup:
    return hr;
}

HRESULT CFrameworkElement::OnDetach(CUIDetachContext& Context)
{
    HRESULT hr = S_OK;
    CNamescope* pNamescope = NULL;

    pNamescope = GetNamescope();
    IFCPTR(pNamescope);

    if(!m_Name.empty())
    {
        IFC(pNamescope->UnregisterName(m_Name.c_str(), this));
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

VOID CFrameworkElement::OnChildAdded(CUIElement* pElement)
{
    HRESULT hr = S_OK;

    IFCPTR(pElement);

    IFC(AddChildVisual(pElement));

    if(IsAttached())
    {
        CUIAttachContext ChildContext(this, GetProviders());

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

    IFCPTR(pName);

    IFC(SetNameInternal(pName));

Cleanup:
    return hr;
}

HRESULT CFrameworkElement::SetNameInternal(const WCHAR* pName)
{
    HRESULT hr = S_OK;
    CNamescope* pNamescope = NULL;

    pNamescope = GetNamescope();

    if(!m_Name.empty())
    {
        if(pNamescope)
        {
            IFC(pNamescope->UnregisterName(pName, this));
        }
    }

    if(pName)
    {
        m_Name.assign(pName);

        if(!m_Name.empty())
        {
            if(pNamescope)
            {
                IFC(pNamescope->RegisterName(pName, this));
            }
        }
    }
    else
    {
        m_Name.clear();
    }

Cleanup:
    return hr;
}

HRESULT CFrameworkElement::SetStyleInternal(CStyle* pStyle)
{
    HRESULT hr = S_OK;

    ReleaseObject(m_Style);

    m_Style = pStyle;

    AddRefObject(m_Style);

    m_StyleDirty = TRUE;

    IFC(EnsureStyle());

Cleanup:
    return hr;
}

HRESULT CFrameworkElement::EnsureStyle()
{
    HRESULT hr = S_OK;

    if(m_StyleDirty)
    {
        if(m_Style)
        {
            if(IsAttached())
            {
                IFC(ApplyStyle(m_Style));
            }
        }
        else
        {
            m_StyleDirty = FALSE;
        }
    }

Cleanup:
    return hr;
}

HRESULT CFrameworkElement::ApplyStyle(CStyle* pStyle)
{
    HRESULT hr = S_OK;

    IFCPTR(pStyle);

    IFC(pStyle->ResolveSetters(this, GetProviders(), &m_SetterResolvedCallback));

Cleanup:
    return hr;
}

HRESULT CFrameworkElement::OnStyleSetterResolved(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;
    CTypeConverter* pTypeConverter = NULL;
    CObjectWithType* pConvertedType = NULL;
    CLayeredValue* pLayeredValue = NULL;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    pTypeConverter = GetTypeConverter();
    IFCPTR(pTypeConverter);

    IFC(GetLayeredValue(pProperty, &pLayeredValue));

    IFC(pLayeredValue->SetStyleValue(pValue, GetProviders()));

    //TODO: Where is the value changed notification supposed to go?
    //TODO: How does this fit with invalidated layered properties?
    IFC(pProperty->OnValueChanged(this));

Cleanup:
    ReleaseObject(pConvertedType);

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

    hr = m_Resources->GetObject(pKey, ppObject);

    if(FAILED(hr))
    {
        CUIElement* pParent = GetParent();

        if(pParent != NULL && pParent->IsTypeOf(TypeIndex::FrameworkElement))
        {
            CFrameworkElement* pParentFrameworkElement = (CFrameworkElement*)pParent;

            IFC(pParentFrameworkElement->FindResource(pKey, ppObject));
        }
    }

Cleanup:
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
    //if(pProperty == &CFrameworkElement::NameProperty)
    //{
    //    *ppLayeredValue = &m_Name;
    //}
    //else if(pProperty == &CFrameworkElement::ResourcesProperty)
    //{
    //    *ppLayeredValue = &m_Resources;
    //}
    //else if(pProperty == &CFrameworkElement::StyleProperty)
    //{
    //    *ppLayeredValue = &m_Style;
    //}
    //else
    //{
        hr = CUIElement::GetLayeredValue(pProperty, ppLayeredValue);
    /*}*/

Cleanup:
    return hr;
}

HRESULT CFrameworkElement::SetValueInternal(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    if(pProperty == &CFrameworkElement::NameProperty)
    {
        IFCEXPECT(pValue->IsTypeOf(TypeIndex::String));

        CStringValue* pString = (CStringValue*)pValue;

        IFC(SetNameInternal(pString->GetValue()));
    }
    else if(pProperty == &CFrameworkElement::ResourcesProperty)
    {
        IFC(E_NOTIMPL);
    }
    else if(pProperty == &CFrameworkElement::StyleProperty)
    {
        IFCEXPECT(pValue->IsTypeOf(TypeIndex::Style));

        CStyle* pStyle = (CStyle*)pValue;

        IFC(SetStyleInternal(pStyle));
    }
    else
    {
        IFC(CUIElement::SetValueInternal(pProperty, pValue));
    }

Cleanup:
    return hr;
}

HRESULT CFrameworkElement::GetValueInternal(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;
    CStringValue* pStringValue = NULL;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(pProperty == &CFrameworkElement::NameProperty)
    {
        IFC(CStringValue::Create(m_Name.c_str(), &pStringValue));

        *ppValue = pStringValue;
        pStringValue = NULL;
    }
    else if(pProperty == &CFrameworkElement::ResourcesProperty)
    {
        *ppValue = m_Resources;
        AddRefObject(m_Resources);
    }
    else if(pProperty == &CFrameworkElement::StyleProperty)
    {
        *ppValue = m_Style;
        AddRefObject(m_Style);
    }
    else
    {
        IFC(CUIElement::GetValueInternal(pProperty, ppValue));
    }

Cleanup:
    ReleaseObject(pStringValue);

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