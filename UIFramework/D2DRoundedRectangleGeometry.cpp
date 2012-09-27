#include "D2DRoundedRectangleGeometry.h"

#if defined(FRAMEWORK_D2D)

CD2DRoundedRectangleGeometry::CD2DRoundedRectangleGeometry(
	) 
	: m_RoundedRectangleGeometry(NULL)
{
}

CD2DRoundedRectangleGeometry::~CD2DRoundedRectangleGeometry(
	)
{
    ReleaseObject(m_RoundedRectangleGeometry);
}

__checkReturn HRESULT 
CD2DRoundedRectangleGeometry::Initialize(
	__in ID2D1RoundedRectangleGeometry* pRoundedRectangleGeometry
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pRoundedRectangleGeometry);

    IFC(CD2DGeometryBase< CRefCountedObjectBase< CRoundedRectangleGraphicsGeometry > >::Initialize(pRoundedRectangleGeometry));

    m_RoundedRectangleGeometry = pRoundedRectangleGeometry;
    AddRefObject(m_RoundedRectangleGeometry);

Cleanup:
    return hr;
}

#endif