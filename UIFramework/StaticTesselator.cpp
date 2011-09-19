#include "StaticTesselator.h"
#include "ErrorChecking.h"

namespace StaticTesselator
{

__checkReturn HRESULT TesselateRectangle(
    const RectF& Rect,
    __in ITesselationSink* pSink
    )
{
    HRESULT hr = S_OK;

    Point2F TopLeft = Point2F(Rect.left, Rect.top);
    Point2F TopRight = Point2F(Rect.right, Rect.top);
    Point2F BottomLeft = Point2F(Rect.left, Rect.bottom);
    Point2F BottomRight = Point2F(Rect.right, Rect.bottom);        
    
    IFC(pSink->AddTriangle(TopLeft, TopRight, BottomLeft));
    IFC(pSink->AddTriangle(BottomLeft, TopRight, BottomRight));
    
Cleanup:
    return hr;
}

__checkReturn HRESULT TesselateRectangle(
    const RectF& Rect,
    const Matrix3X2F& Transform,
    __in ITesselationSink* pSink
    )
{
    HRESULT hr = S_OK;
    
    //TODO: Remove extra redundant transforms.
    Point2F TopLeft = Transform.TransformPoint(Point2F(Rect.left, Rect.top));
    Point2F TopRight = Transform.TransformPoint(Point2F(Rect.right, Rect.top));
    Point2F BottomLeft = Transform.TransformPoint(Point2F(Rect.left, Rect.bottom));
    Point2F BottomRight = Transform.TransformPoint(Point2F(Rect.right, Rect.bottom));        
    
    IFC(pSink->AddTriangle(TopLeft, TopRight, BottomLeft));
    IFC(pSink->AddTriangle(BottomLeft, TopRight, BottomRight));
    
Cleanup:
    return hr;
}
    
}