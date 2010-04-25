#include "FrameworkElement.h"

CFrameworkElement::CFrameworkElement()
{
}

CFrameworkElement::~CFrameworkElement()
{
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
            CUIDetachContext ChildContext(this);

            IFC(pElement->OnDetach(ChildContext));

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
