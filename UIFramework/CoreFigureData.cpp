#include "CoreFigureData.h"

CCoreFigureData::CCoreFigureData(
    )
    : m_pFigureData(NULL)
    , m_FigureDataSize(0)
    , m_pFigurePoints(NULL)
    , m_FigurePointSize(0)
{
}

CCoreFigureData::~CCoreFigureData(
    )
{
    delete [] m_pFigureData;
    delete [] m_pFigurePoints;
}

__checkReturn HRESULT
CCoreFigureData::Initialize(
    __in_ecount(FigureDataSize) FigureDataType::Value* pFigureData,
    UINT32 FigureDataSize,
    __in_ecount(FigurePointSize) Point2F* pFigurePoints,
    UINT32 FigurePointSize
    )
{
    HRESULT hr = S_OK;

    m_pFigureData = new FigureDataType::Value[FigureDataSize];
    IFCOOM(m_pFigureData);

    memcpy(m_pFigureData, pFigureData, FigureDataSize * sizeof(FigureDataType::Value));

    m_FigureDataSize = FigureDataSize;

    m_pFigurePoints = new Point2F[FigurePointSize];
    IFCOOM(m_pFigurePoints);

    memcpy(m_pFigurePoints, pFigurePoints, FigurePointSize * sizeof(Point2F));

    m_FigurePointSize = FigurePointSize;

Cleanup:
    return hr;
}