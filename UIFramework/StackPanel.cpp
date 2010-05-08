#include "StackPanel.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"

CStackPanel::CStackPanel() : m_Orientation(Orientation::Vertical)
{
}

CStackPanel::~CStackPanel()
{
}

HRESULT CStackPanel::Initialize()
{
    HRESULT hr = S_OK;

    IFC(CPanel::Initialize());

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
    SizeF BaseDesiredSize = { 0 };
    SizeF ChildrenSize = { 0 };
    SizeF MaxSize = { FLT_MAX, FLT_MAX };
    CUIElementCollection* pChildCollection = NULL;

    IFC(CPanel::MeasureInternal(AvailableSize, BaseDesiredSize));

    pChildCollection = GetChildCollection();
    IFCPTR(pChildCollection);

    for(UINT i = 0; i < pChildCollection->GetCount(); i++)
    {
        CUIElement* pElement = pChildCollection->GetAtIndex(i);

        IFC(pElement->Measure(MaxSize));

        SizeF ElementDesiredSize = pElement->GetDesiredSize();

        if(m_Orientation == Orientation::Vertical)
        {
            ChildrenSize.width = max(ChildrenSize.width, ElementDesiredSize.width);
            ChildrenSize.height += ElementDesiredSize.height;
        }
        else
        {
            ChildrenSize.width += ElementDesiredSize.width;
            ChildrenSize.height = max(ChildrenSize.height, ElementDesiredSize.height);
        }
    }

    DesiredSize.width = max(ChildrenSize.width, BaseDesiredSize.width);
    DesiredSize.height = max(ChildrenSize.height, BaseDesiredSize.height);

Cleanup:
    return hr;
}

HRESULT CStackPanel::ArrangeInternal(SizeF Size)
{
    HRESULT hr = S_OK;
    Point2F LayoutPoint = { 0 };
    CUIElementCollection* pChildCollection = NULL;

    pChildCollection = GetChildCollection();
    IFCPTR(pChildCollection);

    for(UINT i = 0; i < pChildCollection->GetCount(); i++)
    {
        CUIElement* pElement = pChildCollection->GetAtIndex(i);

        SizeF ElementDesiredSize = pElement->GetDesiredSize();

        IFC(pElement->Arrange(ElementDesiredSize));

        Matrix3X2 VisualTransform = D2D1::Matrix3x2F::Translation(LayoutPoint.x, LayoutPoint.y);
        
        IFC(pElement->SetVisualTransform(VisualTransform));        

        if(m_Orientation == Orientation::Vertical)
        {
            LayoutPoint.y += ElementDesiredSize.height;
        }
        else
        {
            LayoutPoint.x += ElementDesiredSize.width;
        }
    }

    IFC(CPanel::ArrangeInternal(Size));

Cleanup:
    return hr;
}