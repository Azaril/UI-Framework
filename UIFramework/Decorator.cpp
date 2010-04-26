#include "Decorator.h"

CDecorator::CDecorator() : m_Child(NULL)
{
}

CDecorator::~CDecorator()
{
    ReleaseObject(m_Child);
}

HRESULT CDecorator::Initialize()
{
    HRESULT hr = S_OK;

    IFC(CFrameworkElement::Initialize());

Cleanup:
    return hr;
}

HRESULT CDecorator::SetChild(CUIElement* pChild)
{
    HRESULT hr = S_OK;

    if(m_Child != NULL)
    {
        IFC(RemoveLogicalChild(m_Child));

        ReleaseObject(m_Child);
    }

    m_Child = pChild;

    if(m_Child != NULL)
    {
        AddRefObject(m_Child);

        IFC(AddLogicalChild(pChild));
    }

Cleanup:
    return hr;
}

CUIElement* CDecorator::GetChild()
{
    return m_Child;
}