#include "FrameworkElement.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"
#include "BasicTypes.h"

CStaticProperty FrameworkElementProperties[] = 
{
    CStaticProperty( L"Name", TypeIndex::String, StaticPropertyFlags::None )
};

namespace FrameworkElementPropertyIndex
{
    enum Value
    {
        Name
    };
}

CFrameworkElement::CFrameworkElement() : m_Children(NULL),
                                         m_ChildrenSubscriber(*this)
{
}

CFrameworkElement::~CFrameworkElement()
{
    ReleaseObject(m_Children);
}

HRESULT CFrameworkElement::Initialize()
{
    HRESULT hr = S_OK;

    IFC(CUIElement::Initialize());

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

    {
        CUIAttachContext ChildContext(this);

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
        CUIAttachContext ChildContext(this);

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

    IFC(InternalSetName(pName));

Cleanup:
    return hr;
}

HRESULT CFrameworkElement::InternalSetName(const WCHAR* pName)
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

HRESULT CFrameworkElement::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingProperyInformation = NULL;

    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(FrameworkElementProperties, ARRAYSIZE(FrameworkElementProperties), &pStaticInformation))
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

HRESULT CFrameworkElement::SetValue(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    // Check if the property is a static property.
    if(pProperty >= FrameworkElementProperties && pProperty < FrameworkElementProperties + ARRAYSIZE(FrameworkElementProperties))
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        UINT32 Index = (pStaticProperty - FrameworkElementProperties);
        
        switch(Index)
        {
            case FrameworkElementPropertyIndex::Name:
                {
                    IFCEXPECT(pValue->IsTypeOf(TypeIndex::String));

                    CStringValue* pString = (CStringValue*)pValue;

                    IFC(InternalSetName(pString->GetValue()));

                    break;
                }

            default:
                {
                    IFC(E_FAIL);
                }
        }
    }
    else
    {
        IFC(CUIElement::SetValue(pProperty, pValue));
    }

Cleanup:
    return hr;
}

HRESULT CFrameworkElement::GetValue(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;
    CStringValue* pStringValue = NULL;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    // Check if the property is a static property.
    if(pProperty >= FrameworkElementProperties && pProperty < FrameworkElementProperties + ARRAYSIZE(FrameworkElementProperties))
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        UINT32 Index = (pStaticProperty - FrameworkElementProperties);
        
        switch(Index)
        {
            case FrameworkElementPropertyIndex::Name:
                {
                    IFC(CStringValue::Create(m_Name.c_str(), &pStringValue));

                    *ppValue = pStringValue;
                    pStringValue = NULL;

                    break;
                }

            default:
                {
                    IFC(E_FAIL);
                }
        }
    }
    else
    {
        IFC(CUIElement::GetValue(pProperty, ppValue));
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