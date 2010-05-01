#include "StackPanel.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"

//StaticClassProperty StackPanelProperties[] =
//{
//};
//
//StaticClassProperties StackPanelPropertyInformation =
//{
//    StackPanelProperties,
//    ARRAYSIZE(StackPanelProperties)
//};

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

    IFC(CPanel::MeasureInternal(AvailableSize, BaseDesiredSize));

    for(UINT i = 0; i < m_Children->GetCount(); i++)
    {
        CUIElement* pElement = m_Children->GetAtIndex(i);

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

    for(UINT i = 0; i < m_Children->GetCount(); i++)
    {
        CUIElement* pElement = m_Children->GetAtIndex(i);

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

//HRESULT CStackPanel::CreatePropertyInformation(CPropertyInformation** ppInformation)
//{
//    HRESULT hr = S_OK;
//    CStaticPropertyInformation* pStaticInformation = NULL;
//    CPropertyInformation* pBaseInformation = NULL;
//    CDelegatingPropertyInformation* pDelegatingProperyInformation = NULL;
//
//    IFCPTR(ppInformation);
//
//    IFC(CStaticPropertyInformation::Create(&StackPanelPropertyInformation, &pStaticInformation));
//    IFC(CPanel::CreatePropertyInformation(&pBaseInformation));
//    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingProperyInformation));
//
//    *ppInformation = pDelegatingProperyInformation;
//    pDelegatingProperyInformation = NULL;
//
//Cleanup:
//    ReleaseObject(pStaticInformation);
//    ReleaseObject(pBaseInformation);
//    ReleaseObject(pDelegatingProperyInformation);
//
//    return hr;
//}