#include "D2DGeometryProvider.h"
#include "D2DRectangleGeometry.h"
#include "D2DRoundedRectangleGeometry.h"
#include "D2DGeometryBuilder.h"

#if defined(FRAMEWORK_D2D)

CD2DGeometryProvider::CD2DGeometryProvider(
    )
    : m_pFactory(NULL)
{
}

CD2DGeometryProvider::~CD2DGeometryProvider(
    )
{
    ReleaseObject(m_pFactory);
}

__checkReturn HRESULT
CD2DGeometryProvider::Initialize(
    __in ID2D1Factory* pFactory
    )
{
    HRESULT hr = S_OK;

    m_pFactory = pFactory;
    AddRefObject(m_pFactory);

    return hr;
}

__override __checkReturn HRESULT 
CD2DGeometryProvider::CreateRectangleGeometry(
	const RectF& Rectangle, 
	__deref_out CRectangleGraphicsGeometry** ppRectangleGeometry
	)
{
    HRESULT hr = S_OK;
    ID2D1RectangleGeometry* pD2DRectangleGeometry = NULL;
    CD2DRectangleGeometry* pGeometry = NULL;

    IFCPTR(ppRectangleGeometry);

    IFC(m_pFactory->CreateRectangleGeometry(RectFToD2DRectF(&Rectangle), &pD2DRectangleGeometry));

    IFC(CD2DRectangleGeometry::Create(pD2DRectangleGeometry, &pGeometry));

    *ppRectangleGeometry = pGeometry;
    pGeometry = NULL;

Cleanup:
    ReleaseObject(pD2DRectangleGeometry);
    ReleaseObject(pGeometry);

    return hr;
}

__override __checkReturn HRESULT 
CD2DGeometryProvider::CreateRoundedRectangleGeometry(
	const RectF& Rectangle, 
	FLOAT CornerRadius, 
	__deref_out CRoundedRectangleGraphicsGeometry** ppRoundedRectangleGeometry
	)
{
    HRESULT hr = S_OK;
    ID2D1RoundedRectangleGeometry* pD2DRoundedRectangleGeometry = NULL;
    CD2DRoundedRectangleGeometry* pGeometry = NULL;
    D2D1_ROUNDED_RECT RoundedRect = { *RectFToD2DRectF(&Rectangle), CornerRadius, CornerRadius };

    IFCPTR(ppRoundedRectangleGeometry);

    IFC(m_pFactory->CreateRoundedRectangleGeometry(RoundedRect, &pD2DRoundedRectangleGeometry));

    IFC(CD2DRoundedRectangleGeometry::Create(pD2DRoundedRectangleGeometry, &pGeometry));

    *ppRoundedRectangleGeometry = pGeometry;
    pGeometry = NULL;

Cleanup:
    ReleaseObject(pD2DRoundedRectangleGeometry);
    ReleaseObject(pGeometry);

    return hr;
}

__override __checkReturn HRESULT
CD2DGeometryProvider::CreateGeometryBuilder(
    __deref_out CGraphicsGeometryBuilder** ppGeometryBuilder
    )
{
    HRESULT hr = S_OK;
    CD2DGeometryBuilder* pGeometryBuilder = NULL;

    IFC(CD2DGeometryBuilder::Create(m_pFactory, &pGeometryBuilder));

    *ppGeometryBuilder = pGeometryBuilder;
    pGeometryBuilder = NULL;

Cleanup:
    ReleaseObject(pGeometryBuilder);

    return hr;
}

#endif