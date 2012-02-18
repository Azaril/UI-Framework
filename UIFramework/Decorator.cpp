#include "Decorator.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"

//
// Properties
//
namespace DecoratorProperties
{
    enum Value
    {
        Child
    };
}

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( Child );

//
// Properties
//
CStaticProperty CDecorator::ChildProperty(TypeIndex::Decorator, DecoratorProperties::Child, L"Child", TypeIndex::UIElement, StaticPropertyFlags::Content, GET_DEFAULT( Child ), &INSTANCE_CHANGE_CALLBACK( CDecorator, OnChildChanged ));

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CDecorator, OnChildChanged );

CDecorator::CDecorator()
{
}

CDecorator::~CDecorator()
{
}

HRESULT CDecorator::SetChild(CUIElement* pChild)
{
    HRESULT hr = S_OK;

    IFC(SetValue(&CDecorator::ChildProperty, pChild));

Cleanup:
    return hr;
}

HRESULT CDecorator::OnChildChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    if(pOldValue)
    {
        CUIElement* pOldChild = NULL;

        IFC(CastType(pOldValue, &pOldChild));

        pOldChild = (CUIElement*)pOldValue;

        IFC(RemoveLogicalChild(pOldChild));
    }

    if(pNewValue)
    {
        CUIElement* pNewChild = NULL;

        IFC(CastType(pNewValue, &pNewChild));

        pNewChild = (CUIElement*)pNewValue;

        IFC(AddLogicalChild(pNewChild));
    }

Cleanup:
    return hr;
}

HRESULT CDecorator::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingPropertyInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &ChildProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));
    IFC(CFrameworkElement::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingPropertyInformation));

    *ppInformation = pDelegatingPropertyInformation;
    pDelegatingPropertyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingPropertyInformation);

    return hr;
}

HRESULT CDecorator::GetLayeredValue(CProperty* pProperty, CLayeredValue** ppLayeredValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    //TODO: Make this a lookup table rather than requiring a comparison per property.
    if(pProperty == &CDecorator::ChildProperty)
    {
        *ppLayeredValue = &m_Child;
    }
    else
    {
        hr = CFrameworkElement::GetLayeredValue(pProperty, ppLayeredValue);
    }

Cleanup:
    return hr;
}

//
// CDecorator
//
extern "C" UIFRAMEWORK_API
TypeIndex::Value CDecorator_TypeIndex()
{
    return TypeIndex::Decorator;
}

extern "C" UIFRAMEWORK_API
CFrameworkElement* CDecorator_CastTo_CFrameworkElement(CDecorator* pDecorator)
{
    return pDecorator;
}

extern "C" UIFRAMEWORK_API
CDecorator* CObjectWithType_CastTo_CDecorator(CObjectWithType* pObject)
{
    return (pObject->IsTypeOf(TypeIndex::Decorator)) ? (CDecorator*)pObject : NULL;
}