#include "Decorator.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"

StaticClassProperty DecoratorProperties[] =
{
    { L"Child", TypeIndex::UIElement, StaticPropertyFlags::Content }
};

StaticClassProperties DecoratorPropertyInformation =
{
    DecoratorProperties,
    ARRAYSIZE(DecoratorProperties)
};

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

HRESULT CDecorator::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingProperyInformation = NULL;

    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(&DecoratorPropertyInformation, &pStaticInformation));
    IFC(CFrameworkElement::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingProperyInformation));

    *ppInformation = pDelegatingProperyInformation;
    pDelegatingProperyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingProperyInformation);

    return hr;
}

HRESULT CDecorator::SetValue(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    //TODO: Ensure this property actually belongs to this object.

    //TODO: Looking up other than by name would be much better.
    if(wcscmp(pProperty->GetName(), L"Child") == 0)
    {
        IFCEXPECT(pValue->IsTypeOf(TypeIndex::UIElement));

        CUIElement* pUIElement = (CUIElement*)pValue;

        IFC(SetChild(pUIElement));
    }
    else
    {
        IFC(CFrameworkElement::SetValue(pProperty, pValue));
    }

Cleanup:
    return hr;
}

HRESULT CDecorator::GetValue(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    //TODO: Ensure this property actually belongs to this object.

    //TODO: Looking up other than by name would be much better.
    if(wcscmp(pProperty->GetName(), L"Child") == 0)
    {
        *ppValue = m_Child;
        AddRefObject(m_Child);
    }
    else
    {
        IFC(CFrameworkElement::GetValue(pProperty, ppValue));
    }

Cleanup:
    return hr;
}