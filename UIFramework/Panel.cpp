#include "Panel.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"

StaticClassProperty PanelProperties[] =
{
    { L"Children", TypeIndex::UIElement, StaticPropertyFlags::Content | StaticPropertyFlags::Collection }
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

HRESULT CPanel::SetValue(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    //TODO: Ensure this property actually belongs to this object.

    //TODO: Looking up other than by name would be much better.
    if(wcscmp(pProperty->GetName(), L"Children") == 0)
    {
        IFCEXPECT(pValue->IsTypeOf(TypeIndex::UIElementCollection));

        CUIElementCollection* pUIElementCollection = (CUIElementCollection*)pValue;

        //TODO: Implement!
        //IFC(SetChildren(pUIElementCollection));
        __debugbreak();
    }
    else
    {
        IFC(CFrameworkElement::SetValue(pProperty, pValue));
    }

Cleanup:
    return hr;
}

HRESULT CPanel::GetValue(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    //TODO: Ensure this property actually belongs to this object.

    //TODO: Looking up other than by name would be much better.
    if(wcscmp(pProperty->GetName(), L"Children") == 0)
    {
        *ppValue = m_Children;
        AddRefObject(m_Children);
    }
    else
    {
        IFC(CFrameworkElement::GetValue(pProperty, ppValue));
    }

Cleanup:
    return hr;
}