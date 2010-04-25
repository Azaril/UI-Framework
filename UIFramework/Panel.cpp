#include "Panel.h"

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