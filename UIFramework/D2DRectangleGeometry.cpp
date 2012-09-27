#include "D2DRectangleGeometry.h"

#if defined(FRAMEWORK_D2D)

CD2DRectangleGeometry::CD2DRectangleGeometry(
    ) 
    : m_RectangleGeometry(NULL)
{
}

CD2DRectangleGeometry::~CD2DRectangleGeometry(
    )
{
    ReleaseObject(m_RectangleGeometry);
}

__checkReturn HRESULT
CD2DRectangleGeometry::Initialize(
    __in ID2D1RectangleGeometry* pRectangleGeometry
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pRectangleGeometry);

    IFC(CD2DGeometryBase< CRefCountedObjectBase< CRectangleGraphicsGeometry > >::Initialize(pRectangleGeometry));

    m_RectangleGeometry = pRectangleGeometry;
    AddRefObject(m_RectangleGeometry);

Cleanup:
    return hr;
}

#endif