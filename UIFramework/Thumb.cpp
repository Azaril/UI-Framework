#include "Thumb.h"
#include "MouseInputEventArgs.h"
#include "DragDeltaEventArgs.h"

//
// Events
//
CStaticRoutedEvent CThumb::DragStartedEvent(L"DragStarted", RoutingStrategy::Bubbling);
CStaticRoutedEvent CThumb::DragDeltaEvent(L"DragDelta", RoutingStrategy::Bubbling);
CStaticRoutedEvent CThumb::DragCompletedEvent(L"DragCompleted", RoutingStrategy::Bubbling);

CThumb::CThumb() : m_Dragging(FALSE)
{
}

CThumb::~CThumb()
{
}

HRESULT CThumb::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFC(CControl::Initialize(pProviders));

Cleanup:
    return hr;
}

void CThumb::OnMouseLeftButtonDown(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs) 
{
    HRESULT hr = S_OK;
    CMouseButtonEventArgs* pMouseEventArgs = NULL;
    Point2F MouseLocation;

    IFCPTR(pRoutedEventArgs);

    IFC(CastType(pRoutedEventArgs, &pMouseEventArgs));

    m_Dragging = TRUE;

    IFC(pMouseEventArgs->GetLocation(this, &MouseLocation));

    IFC(StartDragging(MouseLocation));

    //TODO: Capture mouse.
    
    pRoutedEventArgs->SetHandled();

Cleanup:
    ;
}

void CThumb::OnMouseMove(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;

    IFCPTR(pRoutedEventArgs);

    if(m_Dragging)
    {
        CMouseEventArgs* pMouseEventArgs = NULL;
        Point2F MouseLocation;

        IFC(CastType(pRoutedEventArgs, &pMouseEventArgs));

        IFC(pMouseEventArgs->GetLocation(this, &MouseLocation));

        IFC(UpdateDrag(MouseLocation));

        pRoutedEventArgs->SetHandled();
    }

Cleanup:
    ;
}

void CThumb::OnMouseLeftButtonUp(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;

    IFCPTR(pRoutedEventArgs);

    if(m_Dragging)
    {
        m_Dragging = FALSE;

        CMouseEventArgs* pMouseEventArgs = NULL;
        Point2F MouseLocation;

        IFC(CastType(pRoutedEventArgs, &pMouseEventArgs));

        IFC(pMouseEventArgs->GetLocation(this, &MouseLocation));

        IFC(StopDragging(MouseLocation));        

        pRoutedEventArgs->SetHandled();
    }

Cleanup:
    ;
}

HRESULT CThumb::StartDragging(Point2F Location)
{
    HRESULT hr = S_OK;
    CRoutedEventArgs* pDragStartEvent = NULL;

    m_LastMouseLocation = Location;

    IFC(CRoutedEventArgs::Create(&DragStartedEvent, &pDragStartEvent));

    IFC(RaiseEvent(pDragStartEvent));

Cleanup:
    ReleaseObject(pDragStartEvent);

    return hr;
}

HRESULT CThumb::UpdateDrag(Point2F Location)
{
    HRESULT hr = S_OK;
    CDragDeltaEventArgs* pDeltaArgs = NULL;

    FLOAT DeltaX = Location.x - m_LastMouseLocation.x;
    FLOAT DeltaY = Location.y - m_LastMouseLocation.y;

    if(DeltaX != 0.0f && DeltaY != 0.0f)
    {
        IFC(CDragDeltaEventArgs::Create(&DragDeltaEvent, DeltaX, DeltaY, &pDeltaArgs));

        IFC(RaiseEvent(pDeltaArgs));

        m_LastMouseLocation = Location;
    }

Cleanup:
    ReleaseObject(pDeltaArgs);

    return hr;
}

HRESULT CThumb::StopDragging(Point2F Location)
{
    HRESULT hr = S_OK;
    CRoutedEventArgs* pDragCompletedEvent = NULL;

    IFC(UpdateDrag(Location));

    IFC(CRoutedEventArgs::Create(&DragCompletedEvent, &pDragCompletedEvent));

    IFC(RaiseEvent(pDragCompletedEvent));

Cleanup:
    ReleaseObject(pDragCompletedEvent);

    return hr;
}

HRESULT CThumb::HitTest(Point2F LocalPoint, CHitTestResult** ppHitTestResult)
{
    HRESULT hr = S_OK;
    CHitTestResult* pVisualHitTestResult = NULL;

    IFCPTR(ppHitTestResult);

    //IFC(m_BorderVisual->HitTest(LocalPoint, &pVisualHitTestResult));

    if(pVisualHitTestResult)
    {
        IFC(CHitTestResult::Create(this, ppHitTestResult));
    }

    *ppHitTestResult = pVisualHitTestResult;

Cleanup:
    ReleaseObject(pVisualHitTestResult);

    return hr;
}