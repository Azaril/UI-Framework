#include "HitTestResult.h"
#include "Visual.h"

CHitTestResult::CHitTestResult(
    ) 
    : m_HitVisual(NULL)
{
}

CHitTestResult::~CHitTestResult(
    )
{
    ReleaseObject(m_HitVisual);
}

__checkReturn HRESULT 
CHitTestResult::Initialize(
    __in CVisual* pVisual
    )
{
    HRESULT hr = S_OK;

    m_HitVisual = pVisual;
    AddRefObject(pVisual);

    return hr;
}

__checkReturn HRESULT 
CHitTestResult::GetHitVisual(
    __deref_out CVisual** ppVisual
    )
{
    HRESULT hr = S_OK;

    IFCPTR(ppVisual);

    *ppVisual = m_HitVisual;
    AddRefObject(m_HitVisual);

Cleanup:
    return hr;
}