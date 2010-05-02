#include "Transform.h"

HRESULT CMatrixTransform::Initialize(const Matrix3X2& Transform)
{
    HRESULT hr = S_OK;

    m_Transform = Transform;

    return hr;
}

HRESULT CMatrixTransform::TransformPoint(const Point2F& Point, Point2F* pTransformedPoint)
{
    HRESULT hr = S_OK;

    IFCPTR(pTransformedPoint);

    using namespace D2D1;

    *pTransformedPoint = Point * m_Transform;

Cleanup:
    return hr;
}