#include "HitTestResult.h"
#include "Visual.h"

CHitTestResult::CHitTestResult() : m_HitVisual(NULL)
{
}

CHitTestResult::~CHitTestResult()
{
    ReleaseObject(m_HitVisual);
}

HRESULT CHitTestResult::Initialize(CVisual* pVisual)
{
    HRESULT hr = S_OK;

    m_HitVisual = pVisual;
    AddRefObject(pVisual);

    return hr;
}

HRESULT CHitTestResult::GetHitVisual(CVisual** ppVisual)
{
    HRESULT hr = S_OK;

    IFCPTR(ppVisual);

    *ppVisual = m_HitVisual;
    AddRefObject(m_HitVisual);

Cleanup:
    return hr;
}