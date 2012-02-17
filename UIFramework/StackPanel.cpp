#include "StackPanel.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"

//
// Properties
//
namespace StackPanelproperties
{
    enum Value
    {
        Orientation
    };
}

//
// Property Defaults
//
DEFINE_GET_DEFAULT( Orientation, Orientation::Value, Orientation::Vertical );

//
// Properties
//
CStaticProperty CStackPanel::OrientationProperty(TypeIndex::StackPanel, StackPanelproperties::Orientation, L"Orientation", TypeIndex::Orientation, StaticPropertyFlags::None, &GET_DEFAULT( Orientation ), &INSTANCE_CHANGE_CALLBACK( CStackPanel, OnOrientationChanged ));

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CStackPanel, OnOrientationChanged );

CStackPanel::CStackPanel() : m_Orientation(this, &CStackPanel::OrientationProperty)
{
}

CStackPanel::~CStackPanel()
{
}

HRESULT CStackPanel::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFC(CPanel::Initialize(pProviders));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CStackPanel::MeasureInternal( 
    const SizeF& AvailableSize,
    SizeF& DesiredSize
    )
{
    HRESULT hr = S_OK;
    SizeF ChildrenSize;
    SizeF MaxSize(std::numeric_limits< FLOAT >::max(), std::numeric_limits< FLOAT >::max());
    CUIElementCollection* pChildCollection = NULL;
    Orientation::Value Orient = Orientation::Vertical;

    pChildCollection = GetChildCollection();
    IFCPTR(pChildCollection);

    IFC(GetEffectiveOrientation(&Orient));

    if(Orient == Orientation::Horizontal)
    {
        MaxSize.height = AvailableSize.height;
    }
    else
    {
        MaxSize.width = AvailableSize.width;
    }

    for(UINT32 i = 0; i < pChildCollection->GetCount(); i++)
    {
        CUIElement* pElement = pChildCollection->GetAtIndex(i);

        IFC(pElement->Measure(MaxSize));

        SizeF ElementDesiredSize = pElement->GetDesiredSize();

        if(Orient == Orientation::Vertical)
        {
            ChildrenSize.width = std::max(ChildrenSize.width, ElementDesiredSize.width);
            ChildrenSize.height += ElementDesiredSize.height;
        }
        else
        {
            ChildrenSize.width += ElementDesiredSize.width;
            ChildrenSize.height = std::max(ChildrenSize.height, ElementDesiredSize.height);
        }
    }

    DesiredSize.width = ChildrenSize.width;
    DesiredSize.height = ChildrenSize.height;

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CStackPanel::ArrangeInternal(
    const SizeF& AvailableSize,
    SizeF& UsedSize 
    )
{
    HRESULT hr = S_OK;
    Point2F LayoutPoint;
    CUIElementCollection* pChildCollection = NULL;
    Orientation::Value Orient = Orientation::Vertical;

    pChildCollection = GetChildCollection();
    IFCPTR(pChildCollection);

    IFC(GetEffectiveOrientation(&Orient));

    for(UINT32 i = 0; i < pChildCollection->GetCount(); i++)
    {
        CUIElement* pElement = pChildCollection->GetAtIndex(i);

        SizeF ElementDesiredSize = pElement->GetDesiredSize();
        RectF ElementBounds(LayoutPoint.x, LayoutPoint.y, LayoutPoint.x + ElementDesiredSize.width, LayoutPoint.y + ElementDesiredSize.height);

        IFC(pElement->Arrange(ElementBounds));

        if(Orient == Orientation::Vertical)
        {
            LayoutPoint.y += ElementDesiredSize.height;
        }
        else
        {
            LayoutPoint.x += ElementDesiredSize.width;
        }
    }

    UsedSize = AvailableSize;

Cleanup:
    return hr;
}

HRESULT CStackPanel::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingPropertyInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &OrientationProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation))
    IFC(CPanel::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingPropertyInformation));

    *ppInformation = pDelegatingPropertyInformation;
    pDelegatingPropertyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingPropertyInformation);

    return hr;
}

HRESULT CStackPanel::OnOrientationChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    IFC(InvalidateMeasure());
    IFC(InvalidateArrange());

Cleanup:
    return hr;
}

HRESULT CStackPanel::GetLayeredValue(CProperty* pProperty, CLayeredValue** ppLayeredValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    //TODO: Make this a lookup table rather than requiring a comparison per property.
    if(pProperty == &CStackPanel::OrientationProperty)
    {
        *ppLayeredValue = &m_Orientation;
    }
    else
    {
        hr = CPanel::GetLayeredValue(pProperty, ppLayeredValue);
    }

Cleanup:
    return hr;
}

HRESULT CStackPanel::GetEffectiveOrientation(Orientation::Value* pOrientation)
{
    HRESULT hr = S_OK;
    COrientationValue* pEffectiveValue = NULL;

    IFCPTR(pOrientation);

    IFC(m_Orientation.GetTypedEffectiveValue(&pEffectiveValue));

    *pOrientation = pEffectiveValue->GetValue();

Cleanup:
    ReleaseObject(pEffectiveValue);

    return hr;
}

//
// CStackPanel
//
extern "C" UIFRAMEWORK_API
TypeIndex::Value CStackPanel_TypeIndex()
{
    return TypeIndex::StackPanel;
}

extern "C" UIFRAMEWORK_API
CPanel* CStackPanel_CastTo_CPanel(CStackPanel* pStackPanel)
{
    return pStackPanel;
}

extern "C" UIFRAMEWORK_API
CStackPanel* CObjectWithType_CastTo_CStackPanel(CObjectWithType* pObject)
{
    return (pObject->IsTypeOf(TypeIndex::StackPanel)) ? (CStackPanel*)pObject : NULL;
}