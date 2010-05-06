#include "Canvas.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"
#include "BasicTypes.h"

//
// Properties
// 
CStaticProperty CCanvas::LeftProperty( L"Left", TypeIndex::Float, StaticPropertyFlags::Attached );
CStaticProperty CCanvas::TopProperty( L"Top", TypeIndex::Float, StaticPropertyFlags::Attached );
CStaticProperty CCanvas::RightProperty( L"Right", TypeIndex::Float, StaticPropertyFlags::Attached );
CStaticProperty CCanvas::BottomProperty( L"Bottom", TypeIndex::Float, StaticPropertyFlags::Attached );

CCanvas::CCanvas()
{
}

CCanvas::~CCanvas()
{
}

HRESULT CCanvas::Initialize()
{
    HRESULT hr = S_OK;

    IFC(CPanel::Initialize());

Cleanup:
    return hr;
}

HRESULT CCanvas::MeasureInternal(SizeF AvailableSize, SizeF& DesiredSize)
{
    HRESULT hr = S_OK;
    SizeF BaseDesiredSize = { 0 };
    SizeF MaxSize = { FLT_MAX, FLT_MAX };
    SizeF MaxSizeNeeded = { 0, 0 };
    CFloatValue* pLeft = NULL;
    CFloatValue* pTop = NULL;
    CFloatValue* pRight = NULL;
    CFloatValue* pBottom = NULL;

    IFC(CPanel::MeasureInternal(AvailableSize, BaseDesiredSize));

    MaxSizeNeeded = BaseDesiredSize;

    for(UINT i = 0; i < m_Children->GetCount(); i++)
    {
        CUIElement* pElement = m_Children->GetAtIndex(i);

        IFC(pElement->Measure(MaxSize));

        IFC(pElement->GetTypedValue(&LeftProperty, &pLeft));
        IFC(pElement->GetTypedValue(&TopProperty, &pTop));
        IFC(pElement->GetTypedValue(&RightProperty, &pRight));
        IFC(pElement->GetTypedValue(&BottomProperty, &pBottom));

        SizeF ElementDesiredSize = pElement->GetDesiredSize();
        SizeF ElementExtentNeeded = { 0 };

        if(pLeft)
        {
            ElementExtentNeeded.width = pLeft->GetValue() + ElementDesiredSize.width;
        }
        else if(pRight)
        {
            ElementExtentNeeded.width = pRight->GetValue();
        }
        else
        {
            ElementExtentNeeded.width = ElementDesiredSize.width;
        }

        if(pTop)
        {
            ElementExtentNeeded.height = pTop->GetValue() + ElementDesiredSize.height;
        }
        else if(pBottom)
        {
            ElementExtentNeeded.height = pBottom->GetValue();
        }
        else
        {
            ElementExtentNeeded.height = ElementDesiredSize.height;
        }

        MaxSizeNeeded.width = max(MaxSizeNeeded.width, ElementExtentNeeded.width);
        MaxSizeNeeded.height = max(MaxSizeNeeded.height, ElementExtentNeeded.height);

        ReleaseObject(pLeft);
        ReleaseObject(pTop);
        ReleaseObject(pRight);
        ReleaseObject(pBottom);
    }

    DesiredSize = MaxSizeNeeded;

Cleanup:
    ReleaseObject(pLeft);
    ReleaseObject(pTop);
    ReleaseObject(pRight);
    ReleaseObject(pBottom);

    return hr;
}

HRESULT CCanvas::ArrangeInternal(SizeF Size)
{
    HRESULT hr = S_OK;
    CFloatValue* pLeft = NULL;
    CFloatValue* pTop = NULL;
    CFloatValue* pRight = NULL;
    CFloatValue* pBottom = NULL;

    for(UINT i = 0; i < m_Children->GetCount(); i++)
    {
        CUIElement* pElement = m_Children->GetAtIndex(i);

        SizeF ElementDesiredSize = pElement->GetDesiredSize();
        SizeF ElementPosition = { 0 };

        IFC(pElement->GetTypedValue(&LeftProperty, &pLeft));
        IFC(pElement->GetTypedValue(&TopProperty, &pTop));
        IFC(pElement->GetTypedValue(&RightProperty, &pRight));
        IFC(pElement->GetTypedValue(&BottomProperty, &pBottom));

        if(pLeft)
        {
            ElementPosition.width = pLeft->GetValue();
        }
        else if(pRight)
        {
            ElementPosition.width = pRight->GetValue() - ElementDesiredSize.width;
        }

        if(pTop)
        {
            ElementPosition.height = pTop->GetValue();
        }
        else if(pBottom)
        {
            ElementPosition.height = pBottom->GetValue() - ElementDesiredSize.height;
        }

        Matrix3X2 VisualTransform = D2D1::Matrix3x2F::Translation(ElementPosition);
        
        IFC(pElement->SetVisualTransform(VisualTransform));

        IFC(pElement->Arrange(ElementDesiredSize));

        ReleaseObject(pLeft);
        ReleaseObject(pTop);
        ReleaseObject(pRight);
        ReleaseObject(pBottom);
    }

    IFC(CPanel::ArrangeInternal(Size));

Cleanup:
    ReleaseObject(pLeft);
    ReleaseObject(pTop);
    ReleaseObject(pRight);
    ReleaseObject(pBottom);

    return hr;
}

HRESULT CCanvas::HitTest(Point2F LocalPoint, CHitTestResult** ppHitTestResult)
{
    HRESULT hr = S_OK;

    IFCPTR(ppHitTestResult);

    *ppHitTestResult = NULL;

Cleanup:
    return hr;
}

HRESULT CCanvas::CreatePropertyInformation(CPropertyInformation** ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingProperyInformation = NULL;

    IFCPTR(ppInformation);

    CStaticProperty* Properties[] = 
    {
        &LeftProperty,
        &TopProperty,
        &RightProperty,
        &BottomProperty
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