#include "StackPanel.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"

CStackPanel::CStackPanel() : m_Orientation(Orientation::Vertical)
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

HRESULT CStackPanel::SetOrientation(Orientation::Value Direction)
{
    HRESULT hr = S_OK;

    m_Orientation = Direction;

    return hr;
}

HRESULT CStackPanel::MeasureInternal(SizeF AvailableSize, SizeF& DesiredSize)
{
    HRESULT hr = S_OK;
    SizeF ChildrenSize;
    SizeF MaxSize(FLT_MAX, FLT_MAX);
    CUIElementCollection* pChildCollection = NULL;

    pChildCollection = GetChildCollection();
    IFCPTR(pChildCollection);

    //TODO: Change to GetEffectiveOrientation
    if(m_Orientation == Orientation::Horizontal)
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

        if(m_Orientation == Orientation::Vertical)
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

HRESULT CStackPanel::ArrangeInternal(SizeF AvailableSize, SizeF& UsedSize)
{
    HRESULT hr = S_OK;
    Point2F LayoutPoint;
    CUIElementCollection* pChildCollection = NULL;

    pChildCollection = GetChildCollection();
    IFCPTR(pChildCollection);

    for(UINT32 i = 0; i < pChildCollection->GetCount(); i++)
    {
        CUIElement* pElement = pChildCollection->GetAtIndex(i);

        SizeF ElementDesiredSize = pElement->GetDesiredSize();
        RectF ElementBounds(LayoutPoint.x, LayoutPoint.y, LayoutPoint.x + ElementDesiredSize.width, LayoutPoint.y + ElementDesiredSize.height);

        IFC(pElement->Arrange(ElementBounds));

        if(m_Orientation == Orientation::Vertical)
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