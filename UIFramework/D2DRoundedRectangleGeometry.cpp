#include "D2DRoundedRectangleGeometry.h"

CD2DRoundedRectangleGeometry::CD2DRoundedRectangleGeometry() : m_RoundedRectangleGeometry(NULL)
{
}

CD2DRoundedRectangleGeometry::~CD2DRoundedRectangleGeometry()
{
    ReleaseObject(m_RoundedRectangleGeometry);
}

HRESULT CD2DRoundedRectangleGeometry::Initialize(ID2D1RoundedRectangleGeometry* pRoundedRectangleGeometry)
{
    HRESULT hr = S_OK;

    IFCPTR(pRoundedRectangleGeometry);

    IFC(CD2DGeometryBase< CRefCountedObjectBase< CRoundedRectangleGeometry > >::Initialize(pRoundedRectangleGeometry));

    m_RoundedRectangleGeometry = pRoundedRectangleGeometry;
    AddRefObject(m_RoundedRectangleGeometry);

Cleanup:
    return hr;
}