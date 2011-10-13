#include "D2DGeometryBuilder.h"
#include "D2DPathGeometry.h"
#include "D2DUtilities.h"

CD2DGeometryBuilder::CD2DGeometryBuilder(
    )
    : m_pPathGeometry(NULL)
    , m_pGeometrySink(NULL)
{
}

CD2DGeometryBuilder::~CD2DGeometryBuilder(
    )
{
    ReleaseObject(m_pPathGeometry);
    ReleaseObject(m_pGeometrySink);
}

__checkReturn HRESULT
CD2DGeometryBuilder::Initialize(
    __in ID2D1Factory* pFactory
    )
{
    HRESULT hr = S_OK;
    ID2D1PathGeometry* pD2DPathGeometry = NULL;

    IFC(pFactory->CreatePathGeometry(&pD2DPathGeometry));

    IFC(CD2DPathGeometry::Create(pD2DPathGeometry, &m_pPathGeometry));

    IFC(pD2DPathGeometry->Open(&m_pGeometrySink));

Cleanup:
    ReleaseObject(pD2DPathGeometry);

    return hr;
}

__override __checkReturn HRESULT 
CD2DGeometryBuilder::GetGraphicsGeometry(
    __deref_out CGraphicsGeometry** ppGeometry
    )
{
    HRESULT hr = S_OK;

    *ppGeometry = m_pPathGeometry;
    AddRefObject(m_pPathGeometry);

    return hr;
}

__override __checkReturn HRESULT 
CD2DGeometryBuilder::Close(
    )
{
    HRESULT hr = S_OK;

    IFCEXPECT(m_pGeometrySink != NULL);

    IFC(m_pGeometrySink->Close());

Cleanup:
    ReleaseObject(m_pGeometrySink);

    return hr;
}

__override __checkReturn HRESULT 
CD2DGeometryBuilder::SetFillMode(
    FillMode::Value fillMode
    )
{
    HRESULT hr = S_OK;

    IFCEXPECT(m_pGeometrySink != NULL);

    m_pGeometrySink->SetFillMode(FillModeToD2DFillMode(fillMode));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT
CD2DGeometryBuilder::BeginFigure(
    const Point2F& startPoint,
    BeginFigureOptions::Value options
    )
{
    HRESULT hr = S_OK;

    IFCEXPECT(m_pGeometrySink != NULL);

    m_pGeometrySink->BeginFigure(*Point2FTOD2DPoint2F(&startPoint), BeginFigureOptionsToD2DOptions(options));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CD2DGeometryBuilder::EndFigure(
    EndFigureOptions::Value options
    )
{
    HRESULT hr = S_OK;

    IFCEXPECT(m_pGeometrySink != NULL);

    m_pGeometrySink->EndFigure(EndFigureOptionsToD2DOptions(options));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CD2DGeometryBuilder::AddLines(
    __in_ecount(PointCount) const Point2F* pPoints,
    UINT32 Points
    )
{
    HRESULT hr = S_OK;

    IFCEXPECT(m_pGeometrySink != NULL);

    m_pGeometrySink->AddLines(Point2FTOD2DPoint2F(pPoints), Points);

Cleanup:
    return hr;
}

__override __checkReturn HRESULT
CD2DGeometryBuilder::AddBeziers(
    __in_ecount(BezierCount) const BezierSegment* pBeziers,
    UINT32 BezierCount
    )
{
    HRESULT hr = S_OK;

    IFCEXPECT(m_pGeometrySink != NULL);

    m_pGeometrySink->AddBeziers(BezierSegmentToD2DBezierSegment(pBeziers), BezierCount);

Cleanup:
    return hr;
}