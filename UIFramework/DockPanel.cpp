#include "DockPanel.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"
#include "BasicTypes.h"

//
// Properties
//
CStaticProperty CDockPanel::DockProperty( L"Dock", TypeIndex::RectangleEdge, StaticPropertyFlags::Attached );
CStaticProperty CDockPanel::LastChildFillProperty( L"LastChildFill", TypeIndex::Bool, StaticPropertyFlags::None );

CDockPanel::CDockPanel() : m_FillLastChild(TRUE)
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

HRESULT CDockPanel::SetFillLastChildInternal(BOOL Fill)
{
    HRESULT hr = S_OK;

    m_FillLastChild = Fill;

    IFC(InvalidateMeasure());
    IFC(InvalidateArrange());

Cleanup:
    return hr;
}

HRESULT CDockPanel::MeasureInternal(SizeF AvailableSize, SizeF& DesiredSize)
{
    HRESULT hr = S_OK;
    SizeF BaseDesiredSize = { 0 };

    IFC(CPanel::MeasureInternal(AvailableSize, BaseDesiredSize));

    for(UINT i = 0; i < m_Children->GetCount(); i++)
    {
        CUIElement* pElement = m_Children->GetAtIndex(i);

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
    UINT32 ChildCount = m_Children->GetCount();

    for(UINT32 i = 0; i < ChildCount; i++)
    {
        BOOL LastChild = (i == ChildCount - 1);
        CUIElement* pElement = m_Children->GetAtIndex(i);

        SizeF ElementDesiredSize = pElement->GetDesiredSize();
        SizeF ElementPosition = { 0 };
        SizeF ElementFinalSize = { 0 };

        IFC(pElement->GetTypedValue(&DockProperty, &pDock));

        if(m_FillLastChild && LastChild)
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
        &DockProperty
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

HRESULT CDockPanel::SetValueInternal(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    if(pProperty == &CDockPanel::LastChildFillProperty)
    {
        IFCEXPECT(pValue->IsTypeOf(TypeIndex::Bool));

        CBoolValue* pBool = (CBoolValue*)pValue;

        IFC(SetFillLastChildInternal(pBool->GetValue()));
    }
    else
    {
        IFC(CPanel::SetValueInternal(pProperty, pValue));
    }

Cleanup:
    return hr;
}

HRESULT CDockPanel::GetValueInternal(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(pProperty == &CDockPanel::LastChildFillProperty)
    {
        IFC(E_NOTIMPL);
    }
    else
    {
        IFC(CPanel::GetValueInternal(pProperty, ppValue));
    }

Cleanup:
    return hr;
}