#include "Panel.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"

CStaticProperty PanelProperties[] = 
{
    CStaticProperty( L"Children", TypeIndex::UIElement, StaticPropertyFlags::Content | StaticPropertyFlags::Collection )
};

namespace PanelPropertyIndex
{
    enum Value
    {
        Children
    };
}

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

    IFC(CStaticPropertyInformation::Create(PanelProperties, ARRAYSIZE(PanelProperties), &pStaticInformation));
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

    // Check if the property is a static property.
    if(pProperty >= PanelProperties && pProperty < PanelProperties + ARRAYSIZE(PanelProperties))
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        UINT32 Index = (pStaticProperty - PanelProperties);
        
        switch(Index)
        {
            case PanelPropertyIndex::Children:
                {
                    IFCEXPECT(pValue->IsTypeOf(TypeIndex::UIElementCollection));

                    CUIElementCollection* pUIElementCollection = (CUIElementCollection*)pValue;

                    //TODO: Implement!
                    //IFC(SetChildren(pUIElementCollection));
                    __debugbreak();

                    break;
                }

            default:
                {
                    IFC(E_FAIL);
                }
        }
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

HRESULT CPanel::HitTest(Point2F LocalPoint, CHitTestResult** ppHitTestResult)
{
    HRESULT hr = S_OK;

    IFCPTR(ppHitTestResult);

    *ppHitTestResult = NULL;

Cleanup:
    return hr;
}