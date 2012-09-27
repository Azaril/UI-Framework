#include "D3D9HWNDRenderTarget.h"

#if defined(FRAMEWORK_D3D9)

#if !defined(_XBOX)

CD3D9HWNDRenderTarget::CD3D9HWNDRenderTarget(
    )
    : m_Width(0)
    , m_Height(0)
    , m_pSwapChain(NULL)
{
}

CD3D9HWNDRenderTarget::~CD3D9HWNDRenderTarget(
    )
{
    ReleaseObject(m_pSwapChain);
}

__checkReturn HRESULT
CD3D9HWNDRenderTarget::Initialize(
    __in IDirect3DDevice9* pDevice,
    __in IDirect3DSwapChain9* pSwapChain,
    __in CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >* pTextureAtlasPool
    )
{
    HRESULT hr = S_OK;
    IDirect3DSurface9* pBackBuffer = NULL;
    D3DSURFACE_DESC backbufferDescription = { };

    SetObject(m_pSwapChain, pSwapChain);

    IFC(m_pSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer));

    IFC(pBackBuffer->GetDesc(&backbufferDescription));

    m_Width = backbufferDescription.Width;
    m_Height = backbufferDescription.Height;

    IFC(CD3D9RenderTarget::Initialize(pDevice, pTextureAtlasPool));

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
    IDirect3DSurface9* pBackBuffer = NULL;

    IFC(m_pSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer));

    IFC(m_pDevice->SetRenderTarget(0, pBackBuffer));

    IFC(m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0));

    IFC(CD3D9RenderTarget::BeginRendering());

    IFC(m_pDevice->BeginScene());

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CD3D9HWNDRenderTarget::EndRendering(
    )
{
    HRESULT hr = S_OK;

    IFC(CD3D9RenderTarget::EndRendering());

    IFC(m_pDevice->EndScene());

    IFC(m_pSwapChain->Present(NULL, NULL, NULL, NULL, NULL));

Cleanup:
    return hr;
}

#endif

#endif