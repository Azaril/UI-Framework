#include "Transform.h"

HRESULT CMatrixTransform::Initialize(const Matrix3X2F& Transform)
{
    HRESULT hr = S_OK;

    m_Transform = Transform;

    return hr;
}

HRESULT CMatrixTransform::TransformPoint(const Point2F& Point, Point2F* pTransformedPoint)
{
    HRESULT hr = S_OK;

    IFCPTR(pTransformedPoint);

    *pTransformedPoint = Point * m_Transform;

Cleanup:
    return hr;
}

HRESULT CMatrixTransform::Invert(CTransform** ppTransform)
{
    HRESULT hr = S_OK;
    Matrix3X2F InvertedMatrix = m_Transform;
    CMatrixTransform* pMatrixTransform = NULL;

    IFCPTR(ppTransform);

    IFCEXPECT(InvertedMatrix.Invert());

    IFC(CMatrixTransform::Create(InvertedMatrix, &pMatrixTransform));

    *ppTransform = pMatrixTransform;
    pMatrixTransform = NULL;

Cleanup:
    ReleaseObject(pMatrixTransform);

    return hr;
}