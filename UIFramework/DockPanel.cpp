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

HRESULT CDockPanel::Initialize()
{
    HRESULT hr = S_OK;

    IFC(CPanel::Initialize());

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
    SizeF BaseDesiredSize = { 0 };
    CUIElementCollection* pChildCollection = NULL;

    IFC(CPanel::MeasureInternal(AvailableSize, BaseDesiredSize));

    pChildCollection = GetChildCollection();
    IFCPTR(pChildCollection);

    for(UINT i = 0; i < pChildCollection->GetCount(); i++)
    {
        CUIElement* pElement = pChildCollection->GetAtIndex(i);

        IFC(pElement->Measure(AvailableSize));
    }

    //TODO: Is it correct for a dock panel to always consume the available size?
    DesiredSize = AvailableSize;

Cleanup:
    return hr;
}

HRESULT CDockPanel::ArrangeInternal(SizeF Size)
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
        SizeF ElementFinalSize = { 0 };

        IFC(pElement->GetTypedValue(&DockProperty, &pDock));

        if(LastChildFill && LastChild)
        {
            ElementFinalSize.height = max(Size.height - (TopSideOffset + BottomSideOffset), 0);
            ElementFinalSize.width = max(Size.width - (LeftSideOffset + RightSideOffset), 0);

            ElementPosition.width = LeftSideOffset;
            ElementPosition.height = TopSideOffset;
        }
        else
        {
            if(pDock == NULL || pDock->GetValue() == RectangleEdge::Left)
            {
                ElementFinalSize.height = max(Size.height - (TopSideOffset + BottomSideOffset), 0);
                ElementFinalSize.width = ElementDesiredSize.width;

                ElementPosition.width = LeftSideOffset;
                ElementPosition.height = TopSideOffset;

                LeftSideOffset += ElementFinalSize.width;
            }
            else if(pDock->GetValue() == RectangleEdge::Right)
            {
                ElementFinalSize.height = max(Size.height - (TopSideOffset + BottomSideOffset), 0);
                ElementFinalSize.width = ElementDesiredSize.width;

                ElementPosition.width = (Size.width - RightSideOffset) - ElementFinalSize.width;
                ElementPosition.height = TopSideOffset;

                RightSideOffset += ElementFinalSize.width;
            }
            else if(pDock->GetValue() == RectangleEdge::Top)
            {
                ElementFinalSize.height = ElementDesiredSize.height;
                ElementFinalSize.width = max(Size.width - (LeftSideOffset + RightSideOffset), 0);

                ElementPosition.width = LeftSideOffset;
                ElementPosition.height = TopSideOffset;

                TopSideOffset += ElementFinalSize.height;
            }
            else 
            {
                ElementFinalSize.height = ElementDesiredSize.height;
                ElementFinalSize.width = max(Size.width - (LeftSideOffset + RightSideOffset), 0);

                ElementPosition.width = LeftSideOffset;
                ElementPosition.height = (Size.height - BottomSideOffset) - ElementFinalSize.height;

                BottomSideOffset += ElementFinalSize.height;
            }
        }

        Matrix3X2 VisualTransform = D2D1::Matrix3x2F::Translation(ElementPosition);
        
        IFC(pElement->SetVisualTransform(VisualTransform));

        IFC(pElement->Arrange(ElementFinalSize));

        ReleaseObject(pDock);
    }

    IFC(CPanel::ArrangeInternal(Size));

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