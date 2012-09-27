#include "D3D11HWNDRenderTarget.h"

#if defined(FRAMEWORK_D3D11)

CD3D11HWNDRenderTarget::CD3D11HWNDRenderTarget(
    )
    : m_Width(0)
    , m_Height(0)
    , m_pSwapChain(NULL)
    , m_pBackBuffer(NULL)
    , m_pBackBufferView(NULL)
{
}

CD3D11HWNDRenderTarget::~CD3D11HWNDRenderTarget(
    )
{
    ReleaseObject(m_pBackBufferView);
    ReleaseObject(m_pBackBuffer);
    ReleaseObject(m_pSwapChain);
}

__checkReturn HRESULT
CD3D11HWNDRenderTarget::Initialize(
    __in ID3D11Device* pDevice,
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

    IFC(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_pBackBuffer));

    IFC(pDevice->CreateRenderTargetView(m_pBackBuffer, NULL, &m_pBackBufferView));

    IFC(CD3D11RenderTarget::Initialize(pDevice, m_pBackBufferView, pTextureAtlasPool));

Cleanup:
    return hr;
}

SizeF 
CD3D11HWNDRenderTarget::GetSize( 
    )
{
    return SizeF((FLOAT)m_Width, (FLOAT)m_Height);
}

__override __checkReturn HRESULT 
CD3D11HWNDRenderTarget::BeginRendering(
    )
{
    HRESULT hr = S_OK;

    m_pImmediateContext->OMSetRenderTargets(1, &m_pBackBufferView, NULL);

    const FLOAT clearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

    m_pImmediateContext->ClearRenderTargetView(m_pBackBufferView, clearColor);

    IFC(CD3D11RenderTarget::BeginRendering());

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CD3D11HWNDRenderTarget::EndRendering(
    )
{
    HRESULT hr = S_OK;

    IFC(CD3D11RenderTarget::EndRendering());

    IFC(m_pSwapChain->Present(0, 0));

Cleanup:
    return hr;
}

#endif