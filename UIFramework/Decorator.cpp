#include "Decorator.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"

//
// Properties
//
CStaticProperty CDecorator::ChildProperty( L"Child", TypeIndex::UIElement, StaticPropertyFlags::Content );

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

    CStaticProperty* Properties[] = 
    {
        &ChildProperty
    };

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));
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

HRESULT CDecorator::SetValueInternal(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    if(pProperty == &CDecorator::ChildProperty)
    {
        IFCEXPECT(pValue->IsTypeOf(TypeIndex::UIElement));

        CUIElement* pUIElement = (CUIElement*)pValue;

        IFC(SetChild(pUIElement));
    }
    else
    {
        IFC(CFrameworkElement::SetValueInternal(pProperty, pValue));
    }

Cleanup:
    return hr;
}

HRESULT CDecorator::GetValueInternal(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(pProperty == &CDecorator::ChildProperty)
    {
        *ppValue = m_Child;
        AddRefObject(m_Child);
    }
    else
    {
        IFC(CFrameworkElement::GetValueInternal(pProperty, ppValue));
    }

Cleanup:
    return hr;
}