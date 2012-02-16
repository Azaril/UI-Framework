#include "CoreFigureBuilder.h"
#include "CoreFigureData.h"
#include "CoreShapeData.h"

CCoreFigureBuilder::CCoreFigureBuilder(
    )
    : m_pShapeData(NULL)
{
}


CCoreFigureBuilder::~CCoreFigureBuilder(
    )
{
    ReleaseObject(m_pShapeData);
}

__checkReturn HRESULT
CCoreFigureBuilder::Initialize(
    __in CCoreShapeData* pShapeData
    )
{
    HRESULT hr = S_OK;

    m_pShapeData = pShapeData;
    AddRefObject(m_pShapeData);

    return hr;
}

__override __checkReturn HRESULT 
CCoreFigureBuilder::Close(
    )
{
    HRESULT hr = S_OK;
    CCoreFigureData* pFigureData = NULL;

    if (!m_FigureDataType.empty())
    {
        IFC(CCoreFigureData::Create(&m_FigureDataType[0], m_FigureDataType.size(), &m_FigurePoints[0], m_FigurePoints.size(), &pFigureData));

        IFC(m_pShapeData->AddFigureData(pFigureData));
    }

Cleanup:
    ReleaseObject(pFigureData);

    return hr;
}

__override __checkReturn HRESULT 
CCoreFigureBuilder::SetFillMode(
    FillMode::Value fillMode
    )
{
    HRESULT hr = S_OK;

    //
    // Fill mode is stored on shape data and should not be added to the figure data.
    //
    IFC(E_UNEXPECTED);

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CCoreFigureBuilder::BeginFigure(
    const Point2F& startPoint,
    BeginFigureOptions::Value options
    )
{
    HRESULT hr = S_OK;

    FigureDataType::Value dataType = FigureDataType::BeginFigure;

    if ((options & BeginFigureOptions::Hollow) == BeginFigureOptions::Hollow)
    {
        dataType = (FigureDataType::Value)(dataType | FigureDataType::OpenHollow);
    }

    m_FigureDataType.push_back(dataType);
    m_FigurePoints.push_back(startPoint);

    return hr;
}

__override __checkReturn HRESULT 
CCoreFigureBuilder::EndFigure(
    EndFigureOptions::Value options
    )
{
    HRESULT hr = S_OK;

    FigureDataType::Value dataType = FigureDataType::EndFigure;

    if ((options & EndFigureOptions::Closed) == EndFigureOptions::Closed)
    {
        dataType = (FigureDataType::Value)(dataType | FigureDataType::EndClosed);
    }

    m_FigureDataType.push_back(dataType);

    return hr;
}

__override __checkReturn HRESULT 
CCoreFigureBuilder::AddLines(
    __in_ecount(PointCount) const Point2F* pPoints,
    UINT32 Points
    )
{
    HRESULT hr = S_OK;

    for (UINT32 i = 0; i < Points; ++i)
    {
        m_FigureDataType.push_back(FigureDataType::Line);

        m_FigurePoints.push_back(pPoints[i]);
    }

    return hr;
}

__override __checkReturn HRESULT 
CCoreFigureBuilder::AddBeziers(
    __in_ecount(BezierCount) const BezierSegment* pBeziers,
    UINT32 BezierCount
    )
{
    HRESULT hr = S_OK;

    for (UINT32 i = 0; i < BezierCount; ++i)
    {
        m_FigureDataType.push_back(FigureDataType::Bezier);

        //TODO: Remove hack for windows.

        m_FigurePoints.push_back(*(Point2F*)&pBeziers[i].point1);
        m_FigurePoints.push_back(*(Point2F*)&pBeziers[i].point2);
        m_FigurePoints.push_back(*(Point2F*)&pBeziers[i].point3);
    }

    return hr;
}