#include "D3D10HWNDRenderTarget.h"

CD3D10HWNDRenderTarget::CD3D10HWNDRenderTarget(
    )
    : m_Width(0)
    , m_Height(0)
    , m_pSwapChain(NULL)
    , m_pBackBuffer(NULL)
    , m_pBackBufferView(NULL)
{
}

CD3D10HWNDRenderTarget::~CD3D10HWNDRenderTarget(
    )
{
    ReleaseObject(m_pBackBufferView);
    ReleaseObject(m_pBackBuffer);
    ReleaseObject(m_pSwapChain);
}

__checkReturn HRESULT
CD3D10HWNDRenderTarget::Initialize(
    __in ID3D10Device* pDevice,
    __in IDXGISwapChain* pSwapChain,
    __in CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >* pTextureAtlasPool
    )
{
    HRESULT hr = S_OK;
    DXGI_SWAP_CHAIN_DESC swapchainDescription = { };

    SetObject(m_pSwapChain, pSwapChain);

    IFC(m_pSwapChain->GetDesc(&swapchainDescription));

    m_Width = swapchainDescription.BufferDesc.Width;
    m_Height = swapchainDescription.BufferDesc.Height;

    IFC(m_pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (void**)&m_pBackBuffer));

    IFC(pDevice->CreateRenderTargetView(m_pBackBuffer, NULL, &m_pBackBufferView));

    IFC(CD3D10RenderTarget::Initialize(pDevice, m_pBackBufferView, pTextureAtlasPool));

Cleanup:
    return hr;
}

SizeF 
CD3D10HWNDRenderTarget::GetSize( 
    )
{
    return SizeF((FLOAT)m_Width, (FLOAT)m_Height);
}

__override __checkReturn HRESULT 
CD3D10HWNDRenderTarget::BeginRendering(
    )
{
    HRESULT hr = S_OK;

    m_pDevice->OMSetRenderTargets(1, &m_pBackBufferView, NULL);

    const FLOAT clearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

    m_pDevice->ClearRenderTargetView(m_pBackBufferView, clearColor);

    IFC(CD3D10RenderTarget::BeginRendering());

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CD3D10HWNDRenderTarget::EndRendering(
    )
{
    HRESULT hr = S_OK;

    IFC(CD3D10RenderTarget::EndRendering());

    IFC(m_pSwapChain->Present(0, 0));

Cleanup:
    return hr;
}