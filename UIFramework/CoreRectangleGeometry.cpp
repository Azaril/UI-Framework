#include "CoreRectangleGeometry.h"

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

    //TODO: Implement.

    return hr;
}

__override __checkReturn HRESULT 
CCoreRectangleGeometry::GetBounds( 
	const Matrix3X2F& Transform,
	__out RectF* pBounds 
	)
{
    HRESULT hr = S_OK;

    //TODO: Implement.

    return hr;
}

__override __checkReturn HRESULT 
CCoreRectangleGeometry::FillContainsPoint( 
	const Point2F& LocalPoint, 
	__out BOOL* pContainsPoint 
	)
{
    HRESULT hr = S_OK;

    //TODO: Implement.

    return hr;
}