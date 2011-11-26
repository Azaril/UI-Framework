#include "D3D10SurfaceRenderTarget.h"

CD3D10SurfaceRenderTarget::CD3D10SurfaceRenderTarget(
    )
    : m_pTexture(NULL)
    , m_pTextureView(NULL)
    , m_Width(0)
    , m_Height(0)
{
}

CD3D10SurfaceRenderTarget::~CD3D10SurfaceRenderTarget(
    )
{
    ReleaseObject(m_pTexture);
    ReleaseObject(m_pTextureView);
}

__checkReturn HRESULT
CD3D10SurfaceRenderTarget::Initialize(
    __in ID3D10Device* pDevice,
    __in ID3D10Texture2D* pTexture,
    __in CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >* pTextureAtlasPool
    )
{
    HRESULT hr = S_OK;
    D3D10_TEXTURE2D_DESC textureDescription = { };

    pTexture->GetDesc(&textureDescription);

    m_Width = textureDescription.Width;
    m_Height = textureDescription.Height;

    IFC(pDevice->CreateRenderTargetView(pTexture, NULL, &m_pTextureView));

    IFC(CD3D10RenderTarget::Initialize(pDevice, m_pTextureView, pTextureAtlasPool));

Cleanup:
    return hr;
}

SizeF 
CD3D10SurfaceRenderTarget::GetSize( 
    )
{
    return SizeF((FLOAT)m_Width, (FLOAT)m_Height);
}

__override __checkReturn HRESULT 
CD3D10SurfaceRenderTarget::BeginRendering(
    )
{
    HRESULT hr = S_OK;

    m_pDevice->OMSetRenderTargets(1, &m_pTextureView, NULL);

    IFC(CD3D10RenderTarget::BeginRendering());

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CD3D10SurfaceRenderTarget::EndRendering(
    )
{
    HRESULT hr = S_OK;

    IFC(CD3D10RenderTarget::EndRendering());

Cleanup:
    return hr;
}