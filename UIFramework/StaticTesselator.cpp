#include "StaticTesselator.h"
#include "ErrorChecking.h"

namespace StaticTesselator
{

__checkReturn HRESULT TesselateRectangle(
    const RectF& rect,
    __in ITesselationSink* pSink
    )
{
    HRESULT hr = S_OK;

    Point2F TopLeft = Point2F(rect.left, rect.top);
    Point2F TopRight = Point2F(rect.right, rect.top);
    Point2F BottomLeft = Point2F(rect.left, rect.bottom);
    Point2F BottomRight = Point2F(rect.right, rect.bottom);        
    
    IFC(pSink->AddTriangle(TopLeft, TopRight, BottomLeft));
    IFC(pSink->AddTriangle(BottomLeft, TopRight, BottomRight));
    
Cleanup:
    return hr;
}

__checkReturn HRESULT TesselateRectangle(
    const RectF& rect,
    const Matrix3X2F& transform,
    __in ITesselationSink* pSink
    )
{
    HRESULT hr = S_OK;
    
    //TODO: Remove extra redundant transforms.
    Point2F TopLeft = transform.TransformPoint(Point2F(rect.left, rect.top));
    Point2F TopRight = transform.TransformPoint(Point2F(rect.right, rect.top));
    Point2F BottomLeft = transform.TransformPoint(Point2F(rect.left, rect.bottom));
    Point2F BottomRight = transform.TransformPoint(Point2F(rect.right, rect.bottom));        
    
    IFC(pSink->AddTriangle(TopLeft, TopRight, BottomLeft));
    IFC(pSink->AddTriangle(BottomLeft, TopRight, BottomRight));
    
Cleanup:
    return hr;
}

__checkReturn HRESULT TesselateRectangleStroke(
    const RectF& rect,
    FLOAT strokeWidth,
    __in ITesselationSink* pSink
    )
{
    HRESULT hr = S_OK;

    FLOAT halfStrokeWidth = strokeWidth / 2.0f;
    
    Point2F OuterTopLeft = Point2F(rect.left - halfStrokeWidth, rect.top - halfStrokeWidth);
    Point2F OuterTopRight = Point2F(rect.right + halfStrokeWidth, rect.top - halfStrokeWidth);
    Point2F OuterBottomLeft = Point2F(rect.left - halfStrokeWidth, rect.bottom + halfStrokeWidth);
    Point2F OuterBottomRight = Point2F(rect.right + halfStrokeWidth , rect.bottom + halfStrokeWidth);
    
    Point2F InnerTopLeft = Point2F(rect.left + halfStrokeWidth, rect.top + halfStrokeWidth);
    Point2F InnerTopRight = Point2F(rect.right - halfStrokeWidth, rect.top + halfStrokeWidth);
    Point2F InnerBottomLeft = Point2F(rect.left + halfStrokeWidth, rect.bottom - halfStrokeWidth);
    Point2F InnerBottomRight = Point2F(rect.right - halfStrokeWidth , rect.bottom - halfStrokeWidth);            
    
    //
    // Top trapezoid.
    //
    IFC(pSink->AddTriangle(OuterTopLeft, OuterTopRight, InnerTopLeft));
    IFC(pSink->AddTriangle(InnerTopLeft, OuterTopRight, InnerTopRight));

    //
    // Right trapezoid.
    //
    IFC(pSink->AddTriangle(OuterTopRight, OuterBottomRight, InnerTopRight));
    IFC(pSink->AddTriangle(InnerTopRight, OuterBottomRight, InnerBottomRight));    

    //
    // Bottom trapezoid.
    //
    IFC(pSink->AddTriangle(OuterBottomRight, OuterBottomLeft, InnerBottomRight));
    IFC(pSink->AddTriangle(InnerBottomRight, OuterBottomLeft, InnerBottomLeft));

    //
    // Left trapezoid.
    //
    IFC(pSink->AddTriangle(OuterBottomLeft, OuterTopLeft, InnerBottomLeft));
    IFC(pSink->AddTriangle(InnerBottomLeft, OuterTopLeft, InnerTopLeft));    
    
Cleanup:
    return hr;
}

__checkReturn HRESULT TesselateRectangleStroke(
    const RectF& rect,
    FLOAT strokeWidth,
    const Matrix3X2F& transform,
    __in ITesselationSink* pSink
    )
{
    HRESULT hr = S_OK;

    FLOAT halfStrokeWidth = strokeWidth / 2.0f;
    
    Point2F TopLeft = transform.TransformPoint(Point2F(rect.left, rect.top));
    Point2F TopRight = transform.TransformPoint(Point2F(rect.right, rect.top));
    Point2F BottomLeft = transform.TransformPoint(Point2F(rect.left, rect.bottom));
    Point2F BottomRight = transform.TransformPoint(Point2F(rect.right, rect.bottom)); 
    
    Point2F TopLeftOutVec = (((TopLeft - BottomLeft) + (TopLeft - TopRight)) / 2.0f).Normalize();
    Point2F TopRightOutVec = (((TopRight - BottomRight) + (TopRight - TopLeft)) / 2.0f).Normalize();
    Point2F BottomLeftOutVec = (((BottomLeft - TopLeft) + (BottomLeft - BottomRight)) / 2.0f).Normalize();    
    Point2F BottomRightOutVec = (((BottomRight - TopRight) + (BottomRight - BottomLeft)) / 2.0f).Normalize();
    
    Point2F OuterTopLeft = TopLeft + (TopLeftOutVec * halfStrokeWidth);
    Point2F OuterTopRight = TopRight + (TopRightOutVec * halfStrokeWidth);
    Point2F OuterBottomLeft = BottomLeft + (BottomLeftOutVec * halfStrokeWidth);
    Point2F OuterBottomRight = BottomRight + (BottomRightOutVec * halfStrokeWidth);
    
    Point2F InnerTopLeft = TopLeft - (TopLeftOutVec * halfStrokeWidth);
    Point2F InnerTopRight = TopRight - (TopRightOutVec * halfStrokeWidth);
    Point2F InnerBottomLeft = BottomLeft - (BottomLeftOutVec * halfStrokeWidth);
    Point2F InnerBottomRight = BottomRight - (BottomRightOutVec * halfStrokeWidth);
    
    //
    // Top trapezoid.
    //
    IFC(pSink->AddTriangle(OuterTopLeft, OuterTopRight, InnerTopLeft));
    IFC(pSink->AddTriangle(InnerTopLeft, OuterTopRight, InnerTopRight));

    //
    // Right trapezoid.
    //
    IFC(pSink->AddTriangle(OuterTopRight, OuterBottomRight, InnerTopRight));
    IFC(pSink->AddTriangle(InnerTopRight, OuterBottomRight, InnerBottomRight));    

    //
    // Bottom trapezoid.
    //
    IFC(pSink->AddTriangle(OuterBottomRight, OuterBottomLeft, InnerBottomRight));
    IFC(pSink->AddTriangle(InnerBottomRight, OuterBottomLeft, InnerBottomLeft));

    //
    // Left trapezoid.
    //
    IFC(pSink->AddTriangle(OuterBottomLeft, OuterTopLeft, InnerBottomLeft));
    IFC(pSink->AddTriangle(InnerBottomLeft, OuterTopLeft, InnerTopLeft));    
    
Cleanup:
    return hr;
}
    
}