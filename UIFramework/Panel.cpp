#include "Panel.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"

//
// Properties
//
CStaticProperty CPanel::ChildrenProperty( L"Children", TypeIndex::UIElement, StaticPropertyFlags::Content | StaticPropertyFlags::Collection );

CPanel::CPanel()
{
}

CPanel::~CPanel()
{
}

HRESULT CPanel::AddChild(CUIElement* pElement)
{
    HRESULT hr = S_OK;

    IFCPTR(pElement);

    IFC(AddLogicalChild(pElement));

Cleanup:
    return hr;
}

HRESULT CPanel::RemoveChild(CUIElement* pElement)
{
    HRESULT hr = S_OK;

    IFCPTR(pElement);

    IFC(RemoveLogicalChild(pElement));

Cleanup:
    return hr;
}

HRESULT CPanel::CreatePropertyInformation(CPropertyInformation** ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingProperyInformation = NULL;

    IFCPTR(ppInformation);

    CStaticProperty* Properties[] = 
    {
        &ChildrenProperty
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

HRESULT CPanel::SetValueInternal(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    if(pProperty == &CPanel::ChildrenProperty)
    {
        IFCEXPECT(pValue->IsTypeOf(TypeIndex::UIElementCollection));

        CUIElementCollection* pUIElementCollection = (CUIElementCollection*)pValue;

        //TODO: Implement!
        //IFC(SetChildren(pUIElementCollection));
        __debugbreak();
    }
    else
    {
        IFC(CFrameworkElement::SetValueInternal(pProperty, pValue));
    }

Cleanup:
    return hr;
}

HRESULT CPanel::GetValueInternal(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(pProperty == &CPanel::ChildrenProperty)
    {
        *ppValue = m_Children;
        AddRefObject(m_Children);
    }
    else
    {
        IFC(CFrameworkElement::GetValueInternal(pProperty, ppValue));
    }

Cleanup:
    return hr;
}

HRESULT CPanel::HitTest(Point2F LocalPoint, CHitTestResult** ppHitTestResult)
{
    HRESULT hr = S_OK;

    IFCPTR(ppHitTestResult);

    *ppHitTestResult = NULL;

Cleanup:
    return hr;
}