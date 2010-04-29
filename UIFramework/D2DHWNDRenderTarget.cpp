#include "D2DHWNDRenderTarget.h"

CD2DHWNDRenderTarget::CD2DHWNDRenderTarget() : m_HWNDRenderTarget(NULL)
{
}

CD2DHWNDRenderTarget::~CD2DHWNDRenderTarget()
{
    ReleaseObject(m_HWNDRenderTarget);
}

HRESULT CD2DHWNDRenderTarget::Initialize(ID2D1HwndRenderTarget* pRenderTarget)
{
    HRESULT hr = S_OK;

    IFCPTR(pRenderTarget);

    IFC(CD2DRenderTarget::Initialize(pRenderTarget));

    m_HWNDRenderTarget = pRenderTarget;
    AddRefObject(m_HWNDRenderTarget);

Cleanup:
    return hr;
}

ID2D1HwndRenderTarget* CD2DHWNDRenderTarget::GetD2DHWNDRenderTarget()
{
    return m_HWNDRenderTarget;
}