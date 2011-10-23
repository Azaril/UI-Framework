#include "TextLayoutEngineMetrics.h"

CTextLayoutEngineMetrics::CTextLayoutEngineMetrics(
    )
{
}

CTextLayoutEngineMetrics::~CTextLayoutEngineMetrics(
    )
{
}

__checkReturn HRESULT
CTextLayoutEngineMetrics::Initialize(
    const RectF& bounds
    )
{
    HRESULT hr = S_OK;

    m_Bounds = bounds;

    return hr;
}

__override __checkReturn HRESULT 
CTextLayoutEngineMetrics::GetBounds(
    __out RectF* pBounds
    )
{
    HRESULT hr = S_OK;

    *pBounds = m_Bounds;

    return hr;
}