#include "CoreRoundedRectangleGeometry.h"

CCoreRoundedRectangleGeometry::CCoreRoundedRectangleGeometry(
    )
{
    m_Rect.left = 0;
    m_Rect.top = 0;
    m_Rect.right = 0;
    m_Rect.bottom = 0;

    m_CornerRadius = 0;
}

CCoreRoundedRectangleGeometry::~CCoreRoundedRectangleGeometry(
    )
{
}

__checkReturn HRESULT
CCoreRoundedRectangleGeometry::Initialize(
    const RectF& rect,
    FLOAT cornerRadius
    )
{
    HRESULT hr = S_OK;

    m_Rect = rect;
    m_CornerRadius = cornerRadius;

    return hr;
}

__override __checkReturn HRESULT 
CCoreRoundedRectangleGeometry::GetBounds(
	__out RectF* pBounds 
	)
{
    HRESULT hr = S_OK;

    //TODO: Implement.

    return hr;
}

__override __checkReturn HRESULT 
CCoreRoundedRectangleGeometry::GetBounds( 
	const Matrix3X2F& Transform,
	__out RectF* pBounds 
	)
{
    HRESULT hr = S_OK;

    //TODO: Implement.

    return hr;
}

__override __checkReturn HRESULT 
CCoreRoundedRectangleGeometry::FillContainsPoint( 
	const Point2F& LocalPoint, 
	__out BOOL* pContainsPoint 
	)
{
    HRESULT hr = S_OK;

    //TODO: Implement.

    return hr;
}