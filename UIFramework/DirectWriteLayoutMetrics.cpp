#include "DirectWriteLayoutMetrics.h"

CDirectWriteLayoutMetrics::CDirectWriteLayoutMetrics()
{
}

CDirectWriteLayoutMetrics::~CDirectWriteLayoutMetrics()
{
}

HRESULT CDirectWriteLayoutMetrics::Initialize(const DWRITE_TEXT_METRICS& Metrics)
{
    HRESULT hr = S_OK;

    m_Metrics = Metrics;

    return hr;
}

HRESULT CDirectWriteLayoutMetrics::GetBounds(RectF* pBounds)
{
    HRESULT hr = S_OK;

    IFCPTR(pBounds);

    pBounds->left = m_Metrics.left;
    pBounds->top = m_Metrics.top;
    pBounds->right = m_Metrics.left + m_Metrics.width;
    pBounds->bottom = m_Metrics.top + m_Metrics.height;

Cleanup:
    return hr;
}