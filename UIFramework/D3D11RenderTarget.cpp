#include "D3D11RenderTarget.h"
#include "D3D11Texture.h"
#include "TextureAtlasWithWhitePixel.h"
#include "StagingTextureWrapper.h"

#include "Shaders/Direct3DHLSL/headers/ps_4_0.h"

#include "Shaders/Direct3DHLSL/headers/vs_4_0.h"

CD3D11RenderTarget::CD3D11RenderTarget(
    )
    : m_pDevice(NULL)
    , m_pImmediateContext(NULL)
    , m_pRenderTargetView(NULL)
    , m_pVertexShader(NULL)
    , m_pPixelShader(NULL)
    , m_pInputLayout(NULL)
    , m_pTransformBuffer(NULL)
    , m_pRasterizerState(NULL)
    , m_pSamplerState(NULL)
    , m_pBlendState(NULL)
    , m_pDepthStencilState(NULL)
{
    for (UINT32 i = 0; i < ARRAYSIZE(m_pVertexBuffers); ++i)
    {
        m_pVertexBuffers[i] = NULL;
    }
}

CD3D11RenderTarget::~CD3D11RenderTarget( 
    )
{
    ReleaseObject(m_pDepthStencilState);
    ReleaseObject(m_pBlendState);
    ReleaseObject(m_pSamplerState);
    ReleaseObject(m_pRasterizerState);
    ReleaseObject(m_pTransformBuffer);
    ReleaseObject(m_pVertexShader);
    ReleaseObject(m_pPixelShader);
    ReleaseObject(m_pInputLayout);

    for (UINT32 i = 0; i < ARRAYSIZE(m_pVertexBuffers); ++i)
    {
        ReleaseObject(m_pVertexBuffers[i]);
    }

    ReleaseObject(m_pRenderTargetView);

    ReleaseObject(m_pImmediateContext);
    ReleaseObject(m_pDevice);
}

__checkReturn HRESULT 
CD3D11RenderTarget::Initialize(
    __in ID3D11Device* pDevice,
    __in ID3D11RenderTargetView* pRenderTargetView,
    __in CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >* pTextureAtlasPool
    )
{
    HRESULT hr = S_OK;
    ID3D11Buffer* pD3DBuffer = NULL;
    CGeometryTesselationSink* pTesselationSink = NULL;
    CTextureAtlasWithWhitePixel< 1 >* pFirstTextureAtlas = NULL;

    SetObject(m_pDevice, pDevice);
    SetObject(m_pRenderTargetView, pRenderTargetView);

    m_pDevice->GetImmediateContext(&m_pImmediateContext);

    for (UINT32 i = 0; i < ARRAYSIZE(m_pVertexBuffers); ++i)
    {
        D3D11_BUFFER_DESC bufferDescription = { };

        bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
        bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDescription.ByteWidth = 32768;
        bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bufferDescription.MiscFlags = 0;

        IFC(m_pDevice->CreateBuffer(&bufferDescription, NULL, &pD3DBuffer));

        IFC(CD3D11VertexBuffer::Create(m_pImmediateContext, pD3DBuffer, &m_pVertexBuffers[i]));     

        ReleaseObject(pD3DBuffer);
    }

    IFC(CGeometryTesselationSink::Create(this, (IVertexBuffer**)m_pVertexBuffers, ARRAYSIZE(m_pVertexBuffers), &pTesselationSink));

    IFC(pTextureAtlasPool->GetOrCreateFirstTextureAtlas(&pFirstTextureAtlas));

    IFC(CRenderTargetBase::Initialize(pTesselationSink, pTextureAtlasPool, (CTextureAtlasView*)pFirstTextureAtlas->GetWhitePixelTexture()));

    IFC(m_pDevice->CreateVertexShader(g_vs_4_0, ARRAYSIZE(g_vs_4_0), NULL, &m_pVertexShader));

    IFC(m_pDevice->CreatePixelShader(g_ps_4_0, ARRAYSIZE(g_ps_4_0), NULL, &m_pPixelShader));

    {
        D3D11_INPUT_ELEMENT_DESC renderVertexDescription[] =
        {
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, RENDERVERTEX_POSITION_OFFSET, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, RENDERVERTEX_COLOR_OFFSET, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT, 0, RENDERVERTEX_TEXCOORDS_OFFSET, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 3, DXGI_FORMAT_R32G32_FLOAT, 0, RENDERVERTEX_MASKCOORDS_OFFSET, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        IFC(m_pDevice->CreateInputLayout(renderVertexDescription, ARRAYSIZE(renderVertexDescription), g_vs_4_0, ARRAYSIZE(g_vs_4_0), &m_pInputLayout));
    }

    {
        D3D11_BUFFER_DESC transformBuffer = { };

        transformBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        transformBuffer.ByteWidth = sizeof(FLOAT) * (4 * 4);
        transformBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        transformBuffer.Usage = D3D11_USAGE_DYNAMIC;
        transformBuffer.MiscFlags = 0;

        IFC(m_pDevice->CreateBuffer(&transformBuffer, NULL, &m_pTransformBuffer));
    }

    {
        D3D11_RASTERIZER_DESC rasterizerDescription = { };

        rasterizerDescription.FillMode = D3D11_FILL_SOLID;
        rasterizerDescription.CullMode = D3D11_CULL_NONE;
        rasterizerDescription.FrontCounterClockwise = FALSE;
        rasterizerDescription.DepthBias = 0;
        rasterizerDescription.DepthBiasClamp = 0;
        rasterizerDescription.SlopeScaledDepthBias = 0;
        rasterizerDescription.DepthClipEnable = FALSE;
        rasterizerDescription.ScissorEnable = FALSE;
        rasterizerDescription.MultisampleEnable = FALSE;
        rasterizerDescription.AntialiasedLineEnable = FALSE;

        IFC(m_pDevice->CreateRasterizerState(&rasterizerDescription, &m_pRasterizerState));
    }

    {
        D3D11_SAMPLER_DESC samplerDescription = { };

        samplerDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        samplerDescription.ComparisonFunc = D3D11_COMPARISON_NEVER;
        samplerDescription.MinLOD = 0;
        samplerDescription.MaxLOD = D3D11_FLOAT32_MAX;

        IFC(m_pDevice->CreateSamplerState(&samplerDescription, &m_pSamplerState));
    }

    {
        D3D11_BLEND_DESC blendDescription = { };

        blendDescription.AlphaToCoverageEnable = FALSE;
        blendDescription.IndependentBlendEnable = FALSE;

        blendDescription.RenderTarget[0].BlendEnable = TRUE;
        
        blendDescription.RenderTarget[0].SrcBlend =  D3D11_BLEND_SRC_ALPHA;
        blendDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        blendDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

        blendDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
        blendDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        blendDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

        blendDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        IFC(m_pDevice->CreateBlendState(&blendDescription, &m_pBlendState));
    }

    {
        D3D11_DEPTH_STENCIL_DESC depthDescription = { };

        depthDescription.DepthEnable = FALSE;
        depthDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depthDescription.StencilEnable = FALSE;
        depthDescription.StencilReadMask = 0xFF;
        depthDescription.StencilWriteMask = 0xFF;
	    depthDescription.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	    depthDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	    depthDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	    depthDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	    depthDescription.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	    depthDescription.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	    depthDescription.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	    depthDescription.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        IFC(m_pDevice->CreateDepthStencilState(&depthDescription, &m_pDepthStencilState));
    }

Cleanup:
    ReleaseObject(pD3DBuffer);
    ReleaseObject(pTesselationSink);
    ReleaseObject(pFirstTextureAtlas);

    return hr;
}

__override __checkReturn HRESULT 
CD3D11RenderTarget::BeginRendering(
    )
{
    HRESULT hr = S_OK;
    FLOAT projectionMatrix[16] = { };
    D3D11_VIEWPORT viewPort = { };
    SizeF viewSize;
    D3D11_MAPPED_SUBRESOURCE mappedTransformBuffer = { };

    IFC(CRenderTargetBase::BeginRendering());

    //
    // Initialize view port.
    //
    viewSize = GetSize();

    viewPort.Width = viewSize.width;
    viewPort.Height = viewSize.height;
    viewPort.MinDepth = 0.0f;
    viewPort.MaxDepth = 0.0f;
    viewPort.TopLeftX = 0;
    viewPort.TopLeftY = 0;

    m_pImmediateContext->RSSetViewports(1, &viewPort);

    //
    // Set shaders.
    //
    m_pImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);

    m_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);

    //
    // Set input layout.
    //
    m_pImmediateContext->IASetInputLayout(m_pInputLayout);

    //
    // Set up orthographic projection matrix.
    // 
    {
        FLOAT nearPlane = 0.001f;
        FLOAT farPlane = 1100.0f;

        FLOAT left = 0.0f;
        FLOAT right = viewSize.width;
        FLOAT top = 0.0f;
        FLOAT bottom = viewSize.height;

        projectionMatrix[0] = 2.0f / (right - left);
        projectionMatrix[1] = 0.0f;
        projectionMatrix[2] = 0.0f;
        projectionMatrix[3] = 0.0f;

        projectionMatrix[4] = 0.0f;
        projectionMatrix[5] = 2.0f / (top - bottom);
        projectionMatrix[6] = 0.0f;
        projectionMatrix[7] = 0.0f;

        projectionMatrix[8] = 0.0f;
        projectionMatrix[9] = 0.0f;
        projectionMatrix[11] = 1.0f / (farPlane - nearPlane);
        projectionMatrix[11] = 0.0f;

        projectionMatrix[12] = (left + right) / (left - right);
        projectionMatrix[13] = (top + bottom) / (bottom - top);
        projectionMatrix[14] = nearPlane / (nearPlane - farPlane);
        projectionMatrix[15] = 1.0f;
    }

    IFC(m_pImmediateContext->Map(m_pTransformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedTransformBuffer));

    memcpy(mappedTransformBuffer.pData, projectionMatrix, sizeof(projectionMatrix));

    m_pImmediateContext->Unmap(m_pTransformBuffer, 0);

    mappedTransformBuffer.pData = NULL;

    m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pTransformBuffer);

    //
    // Set rasterization state (disable culling etc.)
    //
    m_pImmediateContext->RSSetState(m_pRasterizerState);

    //
    // Set sampler state.
    //
    m_pImmediateContext->PSSetSamplers(0, 1, &m_pSamplerState);

    //
    // Set blend state.
    //
    {
        FLOAT blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

        m_pImmediateContext->OMSetBlendState(m_pBlendState, blendFactor, 0xFFFFFFFF);
    }

    //
    // Set depth stencil state.
    //
    m_pImmediateContext->OMSetDepthStencilState(m_pDepthStencilState, 0);

    //
    // Set primitive rendering type.
    //
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

Cleanup:
    if (mappedTransformBuffer.pData != NULL)
    {
        m_pImmediateContext->Unmap(m_pTransformBuffer, 0);
    }

    return hr;
}

__override __checkReturn HRESULT 
CD3D11RenderTarget::EndRendering(
    )
{
    HRESULT hr = S_OK;

    IFC(CRenderTargetBase::EndRendering());

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CD3D11RenderTarget::BindTexture(
    __in ITexture* pTexture
    )
{
    HRESULT hr = S_OK;
    ID3D11ShaderResourceView* pShaderResourceView = NULL;
    ID3D11ShaderResourceView* pNewResourceView = NULL;
    CStagingTextureWrapper* pTextureWrapper = NULL;
    CD3D11Texture* pD3DTexture = NULL;

    pTextureWrapper = (CStagingTextureWrapper*)pTexture;
    pD3DTexture = (CD3D11Texture*)pTextureWrapper->GetTargetTexture();

    pShaderResourceView = pD3DTexture->GetResourceView();

    if (pShaderResourceView == NULL)
    {
        D3D11_TEXTURE2D_DESC textureDescription = { };

        ID3D11Texture2D* pTexture2D = pD3DTexture->GetD3DTexture();

        pTexture2D->GetDesc(&textureDescription);

        D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDescription = { };

        resourceViewDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        resourceViewDescription.Format = textureDescription.Format;
        resourceViewDescription.Texture2D.MipLevels = textureDescription.MipLevels;
        resourceViewDescription.Texture2D.MostDetailedMip = textureDescription.MipLevels - 1;

        IFC(m_pDevice->CreateShaderResourceView(pTexture2D, &resourceViewDescription, &pNewResourceView));

        pD3DTexture->SetResourceView(pNewResourceView);

        pShaderResourceView = pNewResourceView;
    }

    m_pImmediateContext->PSSetShaderResources(0, 1, &pShaderResourceView);

Cleanup:
    ReleaseObject(pNewResourceView);

    return hr;
}

__override __checkReturn HRESULT 
CD3D11RenderTarget::BindMask(
    __in ITexture* pTexture
    )
{
    HRESULT hr = S_OK;
    ID3D11ShaderResourceView* pShaderResourceView = NULL;
    ID3D11ShaderResourceView* pNewResourceView = NULL;
    CStagingTextureWrapper* pTextureWrapper = NULL;
    CD3D11Texture* pD3DTexture = NULL;

    pTextureWrapper = (CStagingTextureWrapper*)pTexture;
    pD3DTexture = (CD3D11Texture*)pTextureWrapper->GetTargetTexture();

    pShaderResourceView = pD3DTexture->GetResourceView();

    if (pShaderResourceView == NULL)
    {
        D3D11_TEXTURE2D_DESC textureDescription = { };

        ID3D11Texture2D* pTexture2D = pD3DTexture->GetD3DTexture();

        pTexture2D->GetDesc(&textureDescription);

        D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDescription = { };

        resourceViewDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        resourceViewDescription.Format = textureDescription.Format;
        resourceViewDescription.Texture2D.MipLevels = textureDescription.MipLevels;
        resourceViewDescription.Texture2D.MostDetailedMip = textureDescription.MipLevels - 1;

        IFC(m_pDevice->CreateShaderResourceView(pTexture2D, &resourceViewDescription, &pNewResourceView));

        pD3DTexture->SetResourceView(pNewResourceView);

        pShaderResourceView = pNewResourceView;
    }

    m_pImmediateContext->PSSetShaderResources(1, 1, &pShaderResourceView);

Cleanup:
    ReleaseObject(pNewResourceView);

    return hr;
}

__override __checkReturn HRESULT 
CD3D11RenderTarget::OnTesselatedGeometryBatch(
    __in IVertexBuffer* pVertexBuffer
    )
{
    HRESULT hr = S_OK;
    CD3D11VertexBuffer* pD3DVertexBuffer = NULL;
    
    pD3DVertexBuffer = (CD3D11VertexBuffer*)pVertexBuffer;

    {
        ID3D11Buffer* pBuffer = pD3DVertexBuffer->GetD3DBuffer();
        const UINT32 stride = sizeof(RenderVertex);
        const UINT32 offset = 0;

        m_pImmediateContext->IASetVertexBuffers(0, 1, &pBuffer, &stride, &offset);
    }

    m_pImmediateContext->Draw(pVertexBuffer->GetVertexCount(), 0);

    return hr;
}

__override __checkReturn HRESULT 
CD3D11RenderTarget::Clear( 
    const ColorF& Color 
    )
{
    HRESULT hr = S_OK;
    FLOAT clearColor[] = { Color.r, Color.g, Color.b, Color.a };

    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);

    return hr;
}