#include "D2DRectangleGeometry.h"

CD2DRectangleGeometry::CD2DRectangleGeometry() : m_RectangleGeometry(NULL)
{
}

CD2DRectangleGeometry::~CD2DRectangleGeometry()
{
    ReleaseObject(m_RectangleGeometry);
}

HRESULT CD2DRectangleGeometry::Initialize(ID2D1RectangleGeometry* pRectangleGeometry)
{
    HRESULT hr = S_OK;

    IFCPTR(pRectangleGeometry);

    IFC(CD2DGeometryBase< CRefCountedObjectBase< CRectangleGeometry > >::Initialize(pRectangleGeometry));

    m_RectangleGeometry = pRectangleGeometry;
    AddRefObject(m_RectangleGeometry);

Cleanup:
    return hr;
}