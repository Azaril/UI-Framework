#include "D3D11SurfaceRenderTarget.h"

CD3D11SurfaceRenderTarget::CD3D11SurfaceRenderTarget(
    )
    : m_pTexture(NULL)
    , m_pTextureView(NULL)
    , m_Width(0)
    , m_Height(0)
{
}

CD3D11SurfaceRenderTarget::~CD3D11SurfaceRenderTarget(
    )
{
    ReleaseObject(m_pTexture);
    ReleaseObject(m_pTextureView);
}

__checkReturn HRESULT
CD3D11SurfaceRenderTarget::Initialize(
    __in ID3D11Device* pDevice,
    __in ID3D11Texture2D* pTexture,
    __in CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >* pTextureAtlasPool
    )
{
    HRESULT hr = S_OK;
    D3D11_TEXTURE2D_DESC textureDescription = { };

    pTexture->GetDesc(&textureDescription);

    m_Width = textureDescription.Width;
    m_Height = textureDescription.Height;

    IFC(pDevice->CreateRenderTargetView(pTexture, NULL, &m_pTextureView));

    IFC(CD3D11RenderTarget::Initialize(pDevice, m_pTextureView, pTextureAtlasPool));

Cleanup:
    return hr;
}

SizeF 
CD3D11SurfaceRenderTarget::GetSize( 
    )
{
    return SizeF((FLOAT)m_Width, (FLOAT)m_Height);
}

__override __checkReturn HRESULT 
CD3D11SurfaceRenderTarget::BeginRendering(
    )
{
    HRESULT hr = S_OK;

    m_pImmediateContext->OMSetRenderTargets(1, &m_pTextureView, NULL);

    IFC(CD3D11RenderTarget::BeginRendering());

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CD3D11SurfaceRenderTarget::EndRendering(
    )
{
    HRESULT hr = S_OK;

    IFC(CD3D11RenderTarget::EndRendering());

Cleanup:
    return hr;
}