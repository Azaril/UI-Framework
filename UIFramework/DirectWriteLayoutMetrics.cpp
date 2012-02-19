#include "DirectWriteLayoutMetrics.h"

CDirectWriteLayoutMetrics::CDirectWriteLayoutMetrics(
	)
{
}

CDirectWriteLayoutMetrics::~CDirectWriteLayoutMetrics(
	)
{
}

__checkReturn HRESULT 
CDirectWriteLayoutMetrics::Initialize(
	const DWRITE_TEXT_METRICS& Metrics
	)
{
    HRESULT hr = S_OK;

    m_Metrics = Metrics;

    return hr;
}

__override __checkReturn HRESULT 
CDirectWriteLayoutMetrics::GetBounds(
	__out RectF* pBounds
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pBounds);

    pBounds->left = m_Metrics.left;
    pBounds->top = m_Metrics.top;
    pBounds->right = m_Metrics.widthIncludingTrailingWhitespace;
    pBounds->bottom = m_Metrics.height;

Cleanup:
    return hr;
}