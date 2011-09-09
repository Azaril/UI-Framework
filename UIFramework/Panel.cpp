#include "Panel.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( Children );

//
// Properties
//
CStaticProperty CPanel::ChildrenProperty( L"Children", TypeIndex::UIElement, StaticPropertyFlags::Content | StaticPropertyFlags::Collection | StaticPropertyFlags::ReadOnly, &GET_DEFAULT( Children ) );

CPanel::CPanel() : m_Children(this, &CPanel::ChildrenProperty)
{
}

CPanel::~CPanel()
{
}

HRESULT CPanel::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFC(CFrameworkElement::Initialize(pProviders));

    IFC(m_Children.SetLocalValue(GetChildCollection()));

Cleanup:
    return hr;
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
    CDelegatingPropertyInformation* pDelegatingPropertyInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &ChildrenProperty
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

HRESULT CPanel::GetLayeredValue(CProperty* pProperty, CLayeredValue** ppLayeredValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    //TODO: Make this a lookup table rather than requiring a comparison per property.
    if(pProperty == &CPanel::ChildrenProperty)
    {
        *ppLayeredValue = &m_Children;
    }
    else
    {
        hr = CFrameworkElement::GetLayeredValue(pProperty, ppLayeredValue);
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

//
// CPanel
//
extern "C" UIFRAMEWORK_API
TypeIndex::Value CPanel_TypeIndex()
{
    return TypeIndex::Panel;
}

extern "C" UIFRAMEWORK_API
CFrameworkElement* CPanel_CastTo_CFrameworkElement(CPanel* pPanel)
{
    return pPanel;
}

extern "C" UIFRAMEWORK_API
CPanel* CObjectWithType_CastTo_CPanel(CObjectWithType* pObject)
{
    return (pObject->IsTypeOf(TypeIndex::Panel)) ? (CPanel*)pObject : NULL;
}

extern "C" UIFRAMEWORK_API
HRESULT CPanel_AddChild(CPanel* pPanel, CUIElement* pChild)
{
    return pPanel->AddChild(pChild);
}

extern "C" UIFRAMEWORK_API
HRESULT CPanel_RemoveChild(CPanel* pPanel, CUIElement* pChild)
{
    return pPanel->RemoveChild(pChild);
}