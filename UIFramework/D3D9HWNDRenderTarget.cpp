#include "D3D9HWNDRenderTarget.h"

CD3D9HWNDRenderTarget::CD3D9HWNDRenderTarget(
    )
    : m_Width(0)
    , m_Height(0)
{
}


CD3D9HWNDRenderTarget::~CD3D9HWNDRenderTarget(
    )
{
}

__checkReturn HRESULT
CD3D9HWNDRenderTarget::Initialize(
    __in IDirect3DDevice9* pDevice
    )
{
    HRESULT hr = S_OK;
    IDirect3DSurface9* pBackBuffer = NULL;
    D3DSURFACE_DESC backbufferDescription = { };

    IFC(pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer));

    IFC(pBackBuffer->GetDesc(&backbufferDescription));

    m_Width = backbufferDescription.Width;
    m_Height = backbufferDescription.Height;

    IFC(CD3D9RenderTarget::Initialize(pDevice));

Cleanup:
    ReleaseObject(pBackBuffer);

    return hr;
}

SizeF 
CD3D9HWNDRenderTarget::GetSize( 
    )
{
    return SizeF((FLOAT)m_Width, (FLOAT)m_Height);
}

__override __checkReturn HRESULT 
CD3D9HWNDRenderTarget::BeginRendering(
    )
{
    HRESULT hr = S_OK;

    IFC(CD3D9RenderTarget::BeginRendering());

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CD3D9HWNDRenderTarget::EndRendering(
    )
{
    HRESULT hr = S_OK;

    IFC(CD3D9RenderTarget::EndRendering());

    IFC(m_pDevice->Present(NULL, NULL, NULL, NULL));

Cleanup:
    return hr;
}