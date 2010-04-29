#include "Panel.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"

StaticClassProperty PanelProperties[] =
{
    { L"Children", TRUE, TypeIndex::UIElement }
};

StaticClassProperties PanelPropertyInformation =
{
    PanelProperties,
    ARRAYSIZE(PanelProperties)
};

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

    IFC(CStaticPropertyInformation::Create(&PanelPropertyInformation, &pStaticInformation));
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