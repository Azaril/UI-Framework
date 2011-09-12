#include "CorePathGraphicsGeometry.h"

CCorePathGraphicsGeometry::CCorePathGraphicsGeometry(
    )
    : m_pShapeData(NULL)
{
}

CCorePathGraphicsGeometry::~CCorePathGraphicsGeometry(
    )
{
    ReleaseObject(m_pShapeData);
}

__checkReturn HRESULT
CCorePathGraphicsGeometry::Initialize(
    __in CCoreShapeData* pShapeData
    )
{
    HRESULT hr = S_OK;

    m_pShapeData = pShapeData;
    AddRefObject(m_pShapeData);

    return hr;
}

__override __checkReturn HRESULT 
CCorePathGraphicsGeometry::GetBounds(
	__out RectF* pBounds 
	)
{
    HRESULT hr = S_OK;

    //TODO: Implement.

    return hr;
}

__override __checkReturn HRESULT 
CCorePathGraphicsGeometry::GetBounds( 
	const Matrix3X2F& Transform,
	__out RectF* pBounds 
	)
{
    HRESULT hr = S_OK;

    //TODO: Implement.

    return hr;
}

__override __checkReturn HRESULT 
CCorePathGraphicsGeometry::FillContainsPoint( 
	const Point2F& LocalPoint, 
	__out BOOL* pContainsPoint 
	)
{
    HRESULT hr = S_OK;

    //TODO: Implement.

    return hr;
}