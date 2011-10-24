#include "D3D9SurfaceRenderTarget.h"

CD3D9SurfaceRenderTarget::CD3D9SurfaceRenderTarget(
    )
    : m_pSurface(NULL)
{
}

CD3D9SurfaceRenderTarget::~CD3D9SurfaceRenderTarget(
    )
{
    ReleaseObject(m_pSurface);
}

__checkReturn HRESULT
CD3D9SurfaceRenderTarget::Initialize(
    __in IDirect3DDevice9* pDevice,
    __in IDirect3DSurface9* pSurface,
    __in CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >* pTextureAtlasPool
    )
{
    HRESULT hr = S_OK;
    D3DSURFACE_DESC surfaceDescription = { };

    SetObject(m_pSurface, pSurface);

    IFC(m_pSurface->GetDesc(&surfaceDescription));

    m_Width = surfaceDescription.Width;
    m_Height = surfaceDescription.Height;

    IFC(CD3D9RenderTarget::Initialize(pDevice, pTextureAtlasPool));

Cleanup:
    return hr;
}

SizeF 
CD3D9SurfaceRenderTarget::GetSize( 
    )
{
    return SizeF((FLOAT)m_Width, (FLOAT)m_Height);
}

__override __checkReturn HRESULT 
CD3D9SurfaceRenderTarget::BeginRendering(
    )
{
    HRESULT hr = S_OK;

    IFC(m_pDevice->SetRenderTarget(0, m_pSurface));

    IFC(CD3D9RenderTarget::BeginRendering());

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CD3D9SurfaceRenderTarget::EndRendering(
    )
{
    HRESULT hr = S_OK;

    IFC(CD3D9RenderTarget::EndRendering());

Cleanup:
    return hr;
}