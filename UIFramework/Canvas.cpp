#include "Canvas.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"
#include "BasicTypes.h"

//
// Properties
//
namespace CanvasProperties
{
    enum Value
    {
        Left,
        Top,
        Right,
        Bottom
    };
}

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( Left );
DEFINE_GET_DEFAULT_NULL( Top );
DEFINE_GET_DEFAULT_NULL( Right );
DEFINE_GET_DEFAULT_NULL( Bottom );

//
// Properties
// 
CStaticProperty CCanvas::LeftProperty(CanvasProperties::Left, L"Left", TypeIndex::Float, StaticPropertyFlags::Attached, &GET_DEFAULT( Left ), &CCanvas::OnLeftChanged);
CStaticProperty CCanvas::TopProperty(CanvasProperties::Top, L"Top", TypeIndex::Float, StaticPropertyFlags::Attached, &GET_DEFAULT( Top ), &CCanvas::OnTopChanged);
CStaticProperty CCanvas::RightProperty(CanvasProperties::Right, L"Right", TypeIndex::Float, StaticPropertyFlags::Attached, &GET_DEFAULT( Right ), &CCanvas::OnRightChanged);
CStaticProperty CCanvas::BottomProperty(CanvasProperties::Bottom, L"Bottom", TypeIndex::Float, StaticPropertyFlags::Attached, &GET_DEFAULT( Bottom ), &CCanvas::OnBottomChanged);

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

__override __checkReturn HRESULT 
CCanvas::MeasureInternal( 
    const SizeF& AvailableSize,
    SizeF& DesiredSize
    )
{
    HRESULT hr = S_OK;
    SizeF MaxSize(std::numeric_limits< FLOAT >::max(), std::numeric_limits< FLOAT >::max());
    CUIElementCollection* pChildCollection = NULL;

    pChildCollection = GetChildCollection();
    IFCPTR(pChildCollection);

    for(UINT32 i = 0; i < pChildCollection->GetCount(); i++)
    {
        CUIElement* pElement = pChildCollection->GetAtIndex(i);

        IFC(pElement->Measure(MaxSize));
    }

    DesiredSize.width = 0;
    DesiredSize.height = 0;

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CCanvas::ArrangeInternal(
    const SizeF& AvailableSize,
    SizeF& UsedSize 
    )
{
    HRESULT hr = S_OK;
    CFloatValue* pLeft = NULL;
    CFloatValue* pTop = NULL;
    CFloatValue* pRight = NULL;
    CFloatValue* pBottom = NULL;
    CUIElementCollection* pChildCollection = NULL;

    pChildCollection = GetChildCollection();
    IFCPTR(pChildCollection);

    for(UINT32 i = 0; i < pChildCollection->GetCount(); i++)
    {
        CUIElement* pElement = pChildCollection->GetAtIndex(i);

        SizeF ElementDesiredSize = pElement->GetDesiredSize();
        SizeF ElementFinalSize;
        SizeF ElementPosition;

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

        RectF ElementBounds(ElementPosition.width, ElementPosition.height, ElementPosition.width + ElementDesiredSize.width, ElementPosition.height + ElementDesiredSize.height);

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
    CDelegatingPropertyInformation* pDelegatingPropertyInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &LeftProperty,
        &TopProperty,
        &RightProperty,
        &BottomProperty
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

__checkReturn HRESULT 
CCanvas::OnLeftChanged(
    __in CPropertyObject* pObject,
    __in_opt CObjectWithType* pOldValue,
    __in_opt CObjectWithType* pNewValue
    )
{
    HRESULT hr = S_OK;
    CUIElement* pUIElement = NULL;

    if (SUCCEEDED(CastType(pObject, &pUIElement)))
    {
        IFC(pUIElement->InvalidateArrange());
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CCanvas::OnRightChanged(
    __in CPropertyObject* pObject,
    __in_opt CObjectWithType* pOldValue,
    __in_opt CObjectWithType* pNewValue
    )
{
    HRESULT hr = S_OK;
    CUIElement* pUIElement = NULL;

    if (SUCCEEDED(CastType(pObject, &pUIElement)))
    {
        IFC(pUIElement->InvalidateArrange());
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CCanvas::OnTopChanged(
    __in CPropertyObject* pObject,
    __in_opt CObjectWithType* pOldValue,
    __in_opt CObjectWithType* pNewValue
    )
{
    HRESULT hr = S_OK;
    CUIElement* pUIElement = NULL;

    if (SUCCEEDED(CastType(pObject, &pUIElement)))
    {
        IFC(pUIElement->InvalidateArrange());
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CCanvas::OnBottomChanged(
    __in CPropertyObject* pObject,
    __in_opt CObjectWithType* pOldValue,
    __in_opt CObjectWithType* pNewValue
    )
{
    HRESULT hr = S_OK;
    CUIElement* pUIElement = NULL;

    if (SUCCEEDED(CastType(pObject, &pUIElement)))
    {
        IFC(pUIElement->InvalidateArrange());
    }

Cleanup:
    return hr;
}

//
// CCanvas
//
extern "C" UIFRAMEWORK_API
TypeIndex::Value CCanvas_TypeIndex()
{
    return TypeIndex::Canvas;
}

extern "C" UIFRAMEWORK_API
CPanel* CCanvas_CastTo_CPanel(CCanvas* pCanvas)
{
    return pCanvas;
}

extern "C" UIFRAMEWORK_API
CCanvas* CObjectWithType_CastTo_CCanvas(CObjectWithType* pObject)
{
    return (pObject->IsTypeOf(TypeIndex::Canvas)) ? (CCanvas*)pObject : NULL;
}