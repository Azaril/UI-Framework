#include "CoreGeometryProvider.h"
#include "ShapeBuilder.h"
#include "CoreGraphicsShapeBuilder.h"
#include "CoreShapeData.h"
#include "CoreRectangleGeometry.h"
#include "CoreRoundedRectangleGeometry.h"

CCoreGeometryProvider::CCoreGeometryProvider(
    )
{
}

CCoreGeometryProvider::~CCoreGeometryProvider(
    )
{
}

__override __checkReturn HRESULT 
CCoreGeometryProvider::CreateRectangleGeometry( 
	const RectF& Rectangle, 
	__deref_out CRectangleGraphicsGeometry** ppRectangleGeometry 
	)
{
    HRESULT hr = S_OK;
    CCoreRectangleGeometry* pCoreRectangleGeometry = NULL;

    IFC(CCoreRectangleGeometry::Create(Rectangle, &pCoreRectangleGeometry));

    *ppRectangleGeometry = pCoreRectangleGeometry;
    pCoreRectangleGeometry = NULL;

Cleanup:
    ReleaseObject(pCoreRectangleGeometry);

    return hr;
}

__override __checkReturn HRESULT 
CCoreGeometryProvider::CreateRoundedRectangleGeometry( 
	const RectF& Rectangle, 
	FLOAT CornerRadius, 
	__deref_out CRoundedRectangleGraphicsGeometry** ppRectangleGeometry 
	)
{
    HRESULT hr = S_OK;
    CCoreRoundedRectangleGeometry* pCoreRoundedRectangleGeometry = NULL;

    IFC(CCoreRoundedRectangleGeometry::Create(Rectangle, CornerRadius, &pCoreRoundedRectangleGeometry));

    *ppRectangleGeometry = pCoreRoundedRectangleGeometry;
    pCoreRoundedRectangleGeometry = NULL;

Cleanup:
    ReleaseObject(pCoreRoundedRectangleGeometry);

    return hr;
}

__checkReturn HRESULT 
CCoreGeometryProvider::CreateGeometryBuilder(
    __deref_out CGraphicsGeometryBuilder** ppGeometryBuilder
    )
{
    HRESULT hr = S_OK;
    CCoreGraphicsShapeBuilder* pShapeBuilder = NULL;

    IFC(CCoreGraphicsShapeBuilder::Create(&pShapeBuilder));

    *ppGeometryBuilder = pShapeBuilder;
    pShapeBuilder = NULL;

Cleanup:
    return hr;
}