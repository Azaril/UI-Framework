#include "CoreRectangleGeometry.h"
#include "StaticTesselator.h"

CCoreRectangleGeometry::CCoreRectangleGeometry(
    )
{
    m_Rect.left = 0;
    m_Rect.top = 0;
    m_Rect.right = 0;
    m_Rect.bottom = 0;
}

CCoreRectangleGeometry::~CCoreRectangleGeometry(
    )
{
}

__checkReturn HRESULT
CCoreRectangleGeometry::Initialize(
    const RectF& rect
    )
{
    HRESULT hr = S_OK;

    m_Rect = rect;

    return hr;
}

__override __checkReturn HRESULT 
CCoreRectangleGeometry::GetBounds(
	__out RectF* pBounds 
	)
{
    HRESULT hr = S_OK;

    *pBounds = m_Rect;

    return hr;
}

__override __checkReturn HRESULT 
CCoreRectangleGeometry::GetBounds( 
	const Matrix3X2F& Transform,
	__out RectF* pBounds 
	)
{
    HRESULT hr = S_OK;
    RectF TransformedBounds;

    Point2F Points[] =
    {
        Point2F(m_Rect.left, m_Rect.top),
        Point2F(m_Rect.right, m_Rect.top),
        Point2F(m_Rect.left, m_Rect.bottom),
        Point2F(m_Rect.right, m_Rect.bottom)
    };
    
    for (UINT32 i = 0; i < ARRAYSIZE(Points); ++i)
    {
        Points[i] = Transform.TransformPoint(Points[i]);
    }
    
    TransformedBounds.left = Points[0].x;
    TransformedBounds.right = Points[0].x;
    TransformedBounds.top = Points[0].y;
    TransformedBounds.bottom = Points[0].y;
    
    for (UINT32 i = 1; i < ARRAYSIZE(Points); ++i)
    {
        TransformedBounds.left = std::min(TransformedBounds.left, Points[0].x);
        TransformedBounds.right = std::max(TransformedBounds.right, Points[0].x);
        TransformedBounds.top = std::min(TransformedBounds.top, Points[0].y);
        TransformedBounds.bottom = std::max(TransformedBounds.bottom, Points[0].y);        
    }
    
    *pBounds = TransformedBounds;

    return hr;
}

__override __checkReturn HRESULT 
CCoreRectangleGeometry::FillContainsPoint( 
	const Point2F& LocalPoint, 
	__out bool* pContainsPoint 
	)
{
    HRESULT hr = S_OK;

    *pContainsPoint = m_Rect.ContainsPoint(LocalPoint);

    return hr;
}

__override __checkReturn HRESULT
CCoreRectangleGeometry::TesselateFill(
    __in ITesselationSink* pSink
    )
{
    HRESULT hr = S_OK;
    
    IFC(StaticTesselator::TesselateRectangle(m_Rect, pSink));
    
Cleanup:
    return hr;
}

__override __checkReturn HRESULT
CCoreRectangleGeometry::TesselateStroke(
    FLOAT strokeThickness,
    __in ITesselationSink* pSink
    )
{
    HRESULT hr = S_OK;
    
    IFC(StaticTesselator::TesselateRectangleStroke(m_Rect, strokeThickness, pSink));
    
Cleanup:
    return hr;
}
