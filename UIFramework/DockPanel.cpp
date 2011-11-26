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

HRESULT CDockPanel::GetEffectiveLastChildFill(bool* pLastChildFill)
{
    HRESULT hr = S_OK;
    CBoolValue* pEffectiveValue = NULL;

    IFCPTR(pLastChildFill);

    IFC(m_LastChildFill.GetTypedEffectiveValue(&pEffectiveValue));

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
    bool LastChildFill = FALSE;

    pChildCollection = GetChildCollection();
    IFCPTR(pChildCollection);

    IFC(GetEffectiveLastChildFill(&LastChildFill));

    ChildCount = pChildCollection->GetCount();

    for(UINT32 i = 0; i < ChildCount; i++)
    {
        CUIElement* pElement = pChildCollection->GetAtIndex(i);

        SizeF ElementAvailableSize(std::max(0.0f, AvailableSize.width - AccumulatedWidth), std::max(0.0f, AvailableSize.height - AccumulatedHeight));

        IFC(pElement->Measure(ElementAvailableSize));

        SizeF ElementDesiredSize = pElement->GetDesiredSize();

        IFC(pElement->GetTypedValue(&DockProperty, &pDock));

        RectangleEdge::Value DockEdge = (pDock != NULL) ? pDock->GetValue() : RectangleEdge::Left;

        switch(DockEdge)
        {
            case RectangleEdge::Left:
            case RectangleEdge::Right:
                {
                    ParentHeight = std::max(ParentHeight, AccumulatedHeight + ElementDesiredSize.height);
                    AccumulatedWidth += ElementDesiredSize.width;

                    break;
                }

            case RectangleEdge::Top:
            case RectangleEdge::Bottom:
                {
                    ParentWidth = std::max(ParentWidth, AccumulatedWidth + ElementDesiredSize.width);
                    AccumulatedHeight += ElementDesiredSize.height;

                    break;
                }
        }

        ReleaseObject(pDock);
    }

    DesiredSize.width = std::max(ParentWidth, AccumulatedWidth);
    DesiredSize.height = std::max(ParentHeight, AccumulatedHeight);

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
    bool LastChildFill = FALSE;

    pChildCollection = GetChildCollection();
    IFCPTR(pChildCollection);

    IFC(GetEffectiveLastChildFill(&LastChildFill));

    ChildCount = pChildCollection->GetCount();

    for(UINT32 i = 0; i < ChildCount; i++)
    {
        bool LastChild = (i == ChildCount - 1);
        CUIElement* pElement = pChildCollection->GetAtIndex(i);

        SizeF ElementDesiredSize = pElement->GetDesiredSize();
        SizeF ElementPosition;
        SizeF ElementAvailableSize;

        IFC(pElement->GetTypedValue(&DockProperty, &pDock));

        if(LastChildFill && LastChild)
        {
            ElementAvailableSize.height = std::max(AvailableSize.height - (TopSideOffset + BottomSideOffset), 0.0f);
            ElementAvailableSize.width = std::max(AvailableSize.width - (LeftSideOffset + RightSideOffset), 0.0f);

            ElementPosition.width = LeftSideOffset;
            ElementPosition.height = TopSideOffset;

            IFC(pElement->Arrange(MakeRect(ElementPosition, ElementAvailableSize)));
        }
        else
        {
            if(pDock == NULL || pDock->GetValue() == RectangleEdge::Left)
            {
                ElementAvailableSize.height = std::max(AvailableSize.height - (TopSideOffset + BottomSideOffset), 0.0f);
                ElementAvailableSize.width = ElementDesiredSize.width;

                ElementPosition.width = LeftSideOffset;
                ElementPosition.height = TopSideOffset;

                IFC(pElement->Arrange(MakeRect(ElementPosition, ElementAvailableSize)));

                LeftSideOffset += ElementDesiredSize.width;
            }
            else if(pDock->GetValue() == RectangleEdge::Right)
            {
                ElementAvailableSize.height = std::max(AvailableSize.height - (TopSideOffset + BottomSideOffset), 0.0f);
                ElementAvailableSize.width = ElementDesiredSize.width;

                ElementPosition.width = (AvailableSize.width - RightSideOffset) - ElementDesiredSize.width;
                ElementPosition.height = TopSideOffset;

                IFC(pElement->Arrange(MakeRect(ElementPosition, ElementAvailableSize)));

                RightSideOffset += ElementDesiredSize.width;
            }
            else if(pDock->GetValue() == RectangleEdge::Top)
            {
                ElementAvailableSize.height = ElementDesiredSize.height;
                ElementAvailableSize.width = std::max(AvailableSize.width - (LeftSideOffset + RightSideOffset), 0.0f);

                ElementPosition.width = LeftSideOffset;
                ElementPosition.height = TopSideOffset;

                IFC(pElement->Arrange(MakeRect(ElementPosition, ElementAvailableSize)));

                TopSideOffset += ElementDesiredSize.height;
            }
            else 
            {
                ElementAvailableSize.height = ElementDesiredSize.height;
                ElementAvailableSize.width = std::max(AvailableSize.width - (LeftSideOffset + RightSideOffset), 0.0f);

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
    CDelegatingPropertyInformation* pDelegatingPropertyInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &DockProperty,
        &LastChildFillProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));
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

//
// CDockPanel
//
extern "C" UIFRAMEWORK_API
TypeIndex::Value CDockPanel_TypeIndex()
{
    return TypeIndex::DockPanel;
}

extern "C" UIFRAMEWORK_API
CPanel* CDockPanel_CastTo_CPanel(CDockPanel* pDockPanel)
{
    return pDockPanel;
}

extern "C" UIFRAMEWORK_API
CDockPanel* CObjectWithType_CastTo_CDockPanel(CObjectWithType* pObject)
{
    return (pObject->IsTypeOf(TypeIndex::DockPanel)) ? (CDockPanel*)pObject : NULL;
}