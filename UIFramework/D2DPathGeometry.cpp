#include "D2DPathGeometry.h"

CD2DPathGeometry::CD2DPathGeometry(
    ) 
    : m_PathGeometry(NULL)
{
}

CD2DPathGeometry::~CD2DPathGeometry()
{
    ReleaseObject(m_PathGeometry);
}

__checkReturn HRESULT 
CD2DPathGeometry::Initialize(
    __in ID2D1PathGeometry* pPathGeometry
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pPathGeometry);

    IFC(CD2DGeometryBase< CRefCountedObjectBase< CPathGraphicsGeometry > >::Initialize(pPathGeometry));

    m_PathGeometry = pPathGeometry;
    AddRefObject(m_PathGeometry);

Cleanup:
    return hr;
}