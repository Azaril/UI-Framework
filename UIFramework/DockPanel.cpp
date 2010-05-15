#include "DockPanel.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"
#include "BasicTypes.h"

//
// Property Defaults
//
DEFINE_GET_DEFAULT( LastChildFill, CBoolValue, TRUE );

//
// Properties
//
CStaticProperty CDockPanel::DockProperty( L"Dock", TypeIndex::RectangleEdge, StaticPropertyFlags::Attached );
CStaticProperty CDockPanel::LastChildFillProperty( L"LastChildFill", TypeIndex::Bool, StaticPropertyFlags::None, &GET_DEFAULT( LastChildFill ), &INSTANCE_CHANGE_CALLBACK( CDockPanel, OnLastChildFillChanged ) );

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CDockPanel, OnLastChildFillChanged );

CDockPanel::CDockPanel() : m_LastChildFill(this, &CDockPanel::LastChildFillProperty)
{
}

CDockPanel::~CDockPanel()
{
}

HRESULT CDockPanel::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFC(CPanel::Initialize(pProviders));

Cleanup:
    return hr;
}

HRESULT CDockPanel::GetEffectiveLastChildFill(BOOL* pLastChildFill)
{
    HRESULT hr = S_OK;
    CBoolValue* pEffectiveValue = NULL;

    IFCPTR(pLastChildFill);

    IFC(m_LastChildFill.GetTypedEffectiveValue(GetProviders(), &pEffectiveValue));

    *pLastChildFill = pEffectiveValue->GetValue();

Cleanup:
    ReleaseObject(pEffectiveValue);

    return hr;
}

HRESULT CDockPanel::OnLastChildFillChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    IFC(InvalidateMeasure());
    IFC(InvalidateArrange());

Cleanup:
    return hr;
}

HRESULT CDockPanel::MeasureInternal(SizeF AvailableSize, SizeF& DesiredSize)
{
    HRESULT hr = S_OK;
    CRectangleEdgeValue* pDock = NULL;
    FLOAT ParentWidth = 0;
    FLOAT ParentHeight = 0;
    FLOAT AccumulatedWidth = 0;
    FLOAT AccumulatedHeight = 0;
    CUIElementCollection* pChildCollection = NULL;
    UINT32 ChildCount = 0;
    BOOL LastChildFill = FALSE;

    pChildCollection = GetChildCollection();
    IFCPTR(pChildCollection);

    IFC(GetEffectiveLastChildFill(&LastChildFill));

    ChildCount = pChildCollection->GetCount();

    for(UINT32 i = 0; i < ChildCount; i++)
    {
        BOOL LastChild = (i == ChildCount - 1);
        CUIElement* pElement = pChildCollection->GetAtIndex(i);

        SizeF ElementAvailableSize = { max(0.0, AvailableSize.width - AccumulatedWidth), max(0.0, AvailableSize.height - AccumulatedHeight) };

        IFC(pElement->Measure(ElementAvailableSize));

        SizeF ElementDesiredSize = pElement->GetDesiredSize();

        IFC(pElement->GetTypedValue(&DockProperty, &pDock));

        RectangleEdge::Value DockEdge = (pDock != NULL) ? pDock->GetValue() : RectangleEdge::Left;

        switch(DockEdge)
        {
            case RectangleEdge::Left:
            case RectangleEdge::Right:
                {
                    ParentHeight = max(ParentHeight, AccumulatedHeight + ElementDesiredSize.height);
                    AccumulatedWidth += ElementDesiredSize.width;

                    break;
                }

            case RectangleEdge::Top:
            case RectangleEdge::Bottom:
                {
                    ParentWidth = max(ParentWidth, AccumulatedWidth + ElementDesiredSize.width);
                    AccumulatedHeight += ElementDesiredSize.height;

                    break;
                }
        }

        ReleaseObject(pDock);
    }

    DesiredSize.width = max(ParentWidth, AccumulatedWidth);
    DesiredSize.height = max(ParentHeight, AccumulatedHeight);

Cleanup:
    ReleaseObject(pDock);

    return hr;
}

HRESULT CDockPanel::ArrangeInternal(SizeF AvailableSize, SizeF& UsedSize)
{
    HRESULT hr = S_OK;
    CRectangleEdgeValue* pDock = NULL;
    FLOAT LeftSideOffset = 0;
    FLOAT RightSideOffset = 0;
    FLOAT TopSideOffset = 0;
    FLOAT BottomSideOffset = 0;
    CUIElementCollection* pChildCollection = NULL;
    UINT32 ChildCount = 0;
    BOOL LastChildFill = FALSE;

    pChildCollection = GetChildCollection();
    IFCPTR(pChildCollection);

    IFC(GetEffectiveLastChildFill(&LastChildFill));

    ChildCount = pChildCollection->GetCount();

    for(UINT32 i = 0; i < ChildCount; i++)
    {
        BOOL LastChild = (i == ChildCount - 1);
        CUIElement* pElement = pChildCollection->GetAtIndex(i);

        SizeF ElementDesiredSize = pElement->GetDesiredSize();
        SizeF ElementPosition = { 0 };
        SizeF ElementAvailableSize = { 0 };

        IFC(pElement->GetTypedValue(&DockProperty, &pDock));

        if(LastChildFill && LastChild)
        {
            ElementAvailableSize.height = max(AvailableSize.height - (TopSideOffset + BottomSideOffset), 0);
            ElementAvailableSize.width = max(AvailableSize.width - (LeftSideOffset + RightSideOffset), 0);

            ElementPosition.width = LeftSideOffset;
            ElementPosition.height = TopSideOffset;

            IFC(pElement->Arrange(MakeRect(ElementPosition, ElementAvailableSize)));
        }
        else
        {
            if(pDock == NULL || pDock->GetValue() == RectangleEdge::Left)
            {
                ElementAvailableSize.height = max(AvailableSize.height - (TopSideOffset + BottomSideOffset), 0);
                ElementAvailableSize.width = ElementDesiredSize.width;

                ElementPosition.width = LeftSideOffset;
                ElementPosition.height = TopSideOffset;

                IFC(pElement->Arrange(MakeRect(ElementPosition, ElementAvailableSize)));

                LeftSideOffset += ElementDesiredSize.width;
            }
            else if(pDock->GetValue() == RectangleEdge::Right)
            {
                ElementAvailableSize.height = max(AvailableSize.height - (TopSideOffset + BottomSideOffset), 0);
                ElementAvailableSize.width = ElementDesiredSize.width;

                ElementPosition.width = (AvailableSize.width - RightSideOffset) - ElementDesiredSize.width;
                ElementPosition.height = TopSideOffset;

                IFC(pElement->Arrange(MakeRect(ElementPosition, ElementAvailableSize)));

                RightSideOffset += ElementDesiredSize.width;
            }
            else if(pDock->GetValue() == RectangleEdge::Top)
            {
                ElementAvailableSize.height = ElementDesiredSize.height;
                ElementAvailableSize.width = max(AvailableSize.width - (LeftSideOffset + RightSideOffset), 0);

                ElementPosition.width = LeftSideOffset;
                ElementPosition.height = TopSideOffset;

                IFC(pElement->Arrange(MakeRect(ElementPosition, ElementAvailableSize)));

                TopSideOffset += ElementDesiredSize.height;
            }
            else 
            {
                ElementAvailableSize.height = ElementDesiredSize.height;
                ElementAvailableSize.width = max(AvailableSize.width - (LeftSideOffset + RightSideOffset), 0);

                ElementPosition.width = LeftSideOffset;
                ElementPosition.height = (AvailableSize.height - BottomSideOffset) - ElementDesiredSize.height;

                IFC(pElement->Arrange(MakeRect(ElementPosition, ElementAvailableSize)));

                BottomSideOffset += ElementDesiredSize.height;
            }
        }

        ReleaseObject(pDock);
    }

    UsedSize = AvailableSize;

Cleanup:
    ReleaseObject(pDock);

    return hr;
}

HRESULT CDockPanel::HitTest(Point2F LocalPoint, CHitTestResult** ppHitTestResult)
{
    HRESULT hr = S_OK;

    IFCPTR(ppHitTestResult);

    *ppHitTestResult = NULL;

Cleanup:
    return hr;
}

HRESULT CDockPanel::CreatePropertyInformation(CPropertyInformation** ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingProperyInformation = NULL;

    IFCPTR(ppInformation);

    CStaticProperty* Properties[] = 
    {
        &DockProperty,
        &LastChildFillProperty
    };

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));
    IFC(CPanel::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingProperyInformation));

    *ppInformation = pDelegatingProperyInformation;
    pDelegatingProperyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingProperyInformation);

    return hr;
}

HRESULT CDockPanel::GetLayeredValue(CProperty* pProperty, CLayeredValue** ppLayeredValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    //TODO: Make this a lookup table rather than requiring a comparison per property.
    if(pProperty == &CDockPanel::LastChildFillProperty)
    {
        *ppLayeredValue = &m_LastChildFill;
    }
    else
    {
        hr = CPanel::GetLayeredValue(pProperty, ppLayeredValue);
    }

Cleanup:
    return hr;
}