#include "D2DSurfaceRenderTarget.h"

CD2DSurfaceRenderTarget::CD2DSurfaceRenderTarget(
	) 
	: m_Surface(NULL)
{
}

CD2DSurfaceRenderTarget::~CD2DSurfaceRenderTarget(
	)
{
    ReleaseObject(m_Surface);
}

__checkReturn HRESULT 
CD2DSurfaceRenderTarget::Initialize(
	__in IDXGISurface* pSurface, 
	__in ID2D1RenderTarget* pRenderTarget
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pRenderTarget);

    IFC(CD2DRenderTarget::Initialize(pRenderTarget));

    m_Surface = pSurface;
    AddRefObject(pSurface);

Cleanup:
    return hr;
}