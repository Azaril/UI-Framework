#include "FrameworkElement.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"

//StaticClassProperty FrameworkElementProperties[] =
//{
//};
//
//StaticClassProperties FrameworkElementPropertyInformation =
//{
//    FrameworkElementProperties,
//    ARRAYSIZE(FrameworkElementProperties)
//};

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

    IFC(CUIElement::OnAttach(Context));

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

    IFC(CUIElement::OnDetach(Context));

    {
        CUIDetachContext ChildContext(this);

        for(UINT i = 0; i < m_Children->GetCount(); i++)
        {
            CUIElement* pElement = m_Children->GetAtIndex(i);

            IFC(pElement->OnDetach(ChildContext));
        }
    }

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

//HRESULT CFrameworkElement::CreatePropertyInformation(CPropertyInformation **ppInformation)
//{
//    HRESULT hr = S_OK;
//    CStaticPropertyInformation* pStaticInformation = NULL;
//    CPropertyInformation* pBaseInformation = NULL;
//    CDelegatingPropertyInformation* pDelegatingProperyInformation = NULL;
//
//    IFCPTR(ppInformation);
//
//    IFC(CStaticPropertyInformation::Create(&FrameworkElementPropertyInformation, &pStaticInformation));
//    IFC(CUIElement::CreatePropertyInformation(&pBaseInformation));
//    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingProperyInformation));
//
//    *ppInformation = pDelegatingProperyInformation;
//    pDelegatingProperyInformation = NULL;
//
//Cleanup:
//    ReleaseObject(pStaticInformation);
//    ReleaseObject(pBaseInformation);
//    ReleaseObject(pDelegatingProperyInformation);
//
//    return hr;
//}

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