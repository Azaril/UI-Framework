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

CFrameworkElement::CFrameworkElement()
{
}

CFrameworkElement::~CFrameworkElement()
{
    Finalize();
}

HRESULT CFrameworkElement::Finalize()
{
    HRESULT hr = S_OK;

    for(ChildCollection::iterator It = m_Children.begin(); It != m_Children.end(); ++It)
    {
        (*It)->Release();
    }

    m_Children.clear();

    return hr;
}

HRESULT CFrameworkElement::OnAttach(CUIAttachContext& Context)
{
    HRESULT hr = S_OK;

    IFC(CUIElement::OnAttach(Context));

    {
        CUIAttachContext ChildContext(this);

        for(ChildCollection::iterator It = m_Children.begin(); It != m_Children.end(); ++It)
        {
            IFC((*It)->OnAttach(ChildContext));
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

        for(ChildCollection::iterator It = m_Children.begin(); It != m_Children.end(); ++It)
        {
            IFC((*It)->OnDetach(ChildContext));
        }
    }

Cleanup:
    return hr;
}

HRESULT CFrameworkElement::AddLogicalChild(CUIElement* pElement)
{
    HRESULT hr = S_OK;

    IFCPTR(pElement);

    for(ChildCollection::iterator It = m_Children.begin(); It != m_Children.end(); ++It)
    {
        if(*It == pElement)
        {
            IFC(E_FAIL);
        }
    }

    IFC(AddChildVisual(pElement));

    m_Children.push_back(pElement);

    AddRefObject(pElement);

    if(IsAttached())
    {
        CUIAttachContext ChildContext(this);

        IFC(pElement->OnAttach(ChildContext));
    }

Cleanup:
    return hr;
}

HRESULT CFrameworkElement::RemoveLogicalChild(CUIElement* pElement)
{
    HRESULT hr = S_OK;

    IFCPTR(pElement);

    for(ChildCollection::iterator It = m_Children.begin(); It != m_Children.end(); ++It)
    {
        if(*It == pElement)
        {
            if(IsAttached())
            {
                CUIDetachContext ChildContext(this);

                IFC(pElement->OnDetach(ChildContext));
            }

            (*It)->Release();

            m_Children.erase(It);

            IFC(RemoveChildVisual(pElement));

            goto Cleanup;
        }
    }

    IFC(E_FAIL);

Cleanup:
    return hr;
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