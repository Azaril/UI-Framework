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

HRESULT CCanvas::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFC(CPanel::Initialize(pProviders));

Cleanup:
    return hr;
}

HRESULT CCanvas::MeasureInternal(SizeF AvailableSize, SizeF& DesiredSize)
{
    HRESULT hr = S_OK;
    SizeF MaxSize = { FLT_MAX, FLT_MAX };
    CUIElementCollection* pChildCollection = NULL;

    pChildCollection = GetChildCollection();
    IFCPTR(pChildCollection);

    for(UINT i = 0; i < pChildCollection->GetCount(); i++)
    {
        CUIElement* pElement = pChildCollection->GetAtIndex(i);

        IFC(pElement->Measure(MaxSize));
    }

    DesiredSize.width = 0;
    DesiredSize.height = 0;

Cleanup:
    return hr;
}

HRESULT CCanvas::ArrangeInternal(SizeF AvailableSize, SizeF& UsedSize)
{
    HRESULT hr = S_OK;
    CFloatValue* pLeft = NULL;
    CFloatValue* pTop = NULL;
    CFloatValue* pRight = NULL;
    CFloatValue* pBottom = NULL;
    CUIElementCollection* pChildCollection = NULL;

    pChildCollection = GetChildCollection();
    IFCPTR(pChildCollection);

    for(UINT i = 0; i < pChildCollection->GetCount(); i++)
    {
        CUIElement* pElement = pChildCollection->GetAtIndex(i);

        SizeF ElementDesiredSize = pElement->GetDesiredSize();
        SizeF ElementFinalSize = { 0 };
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

        RectF ElementBounds = { ElementPosition.width, ElementPosition.height, ElementPosition.width + ElementDesiredSize.width, ElementPosition.height + ElementDesiredSize.height };

        IFC(pElement->Arrange(ElementBounds));

        ElementFinalSize = pElement->GetFinalSize();

        ReleaseObject(pLeft);
        ReleaseObject(pTop);
        ReleaseObject(pRight);
        ReleaseObject(pBottom);
    }

    UsedSize = AvailableSize;

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