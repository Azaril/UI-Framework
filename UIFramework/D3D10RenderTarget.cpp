#include "D3D10RenderTarget.h"
#include "D3D10Texture.h"
#include "TextureAtlasWithWhitePixel.h"
#include "StagingTextureWrapper.h"

#include "Shaders/Direct3DHLSL/headers/ps_4_0.h"

#include "Shaders/Direct3DHLSL/headers/vs_4_0.h"

CD3D10RenderTarget::CD3D10RenderTarget(
    )
    : m_pDevice(NULL)
    , m_pRenderTargetView(NULL)
    , m_pVertexShader(NULL)
    , m_pPixelShader(NULL)
    , m_pInputLayout(NULL)
    , m_pTransformBuffer(NULL)
    , m_pRasterizerState(NULL)
    , m_pSamplerState(NULL)
{
    for (UINT32 i = 0; i < ARRAYSIZE(m_pVertexBuffers); ++i)
    {
        m_pVertexBuffers[i] = NULL;
    }
}

CD3D10RenderTarget::~CD3D10RenderTarget( 
    )
{
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

    ReleaseObject(m_pDevice);
}

__checkReturn HRESULT 
CD3D10RenderTarget::Initialize(
    __in ID3D10Device* pDevice,
    __in ID3D10RenderTargetView* pRenderTargetView,
    __in CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >* pTextureAtlasPool
    )
{
    HRESULT hr = S_OK;
    ID3D10Buffer* pD3DBuffer = NULL;
    CGeometryTesselationSink* pTesselationSink = NULL;
    CTextureAtlasWithWhitePixel< 1 >* pFirstTextureAtlas = NULL;

    SetObject(m_pDevice, pDevice);
    SetObject(m_pRenderTargetView, pRenderTargetView);

    for (UINT32 i = 0; i < ARRAYSIZE(m_pVertexBuffers); ++i)
    {
        D3D10_BUFFER_DESC bufferDescription = { };

        bufferDescription.Usage = D3D10_USAGE_DYNAMIC;
        bufferDescription.BindFlags = D3D10_BIND_VERTEX_BUFFER;
        bufferDescription.ByteWidth = 32768;
        bufferDescription.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
        bufferDescription.MiscFlags = 0;

        IFC(m_pDevice->CreateBuffer(&bufferDescription, NULL, &pD3DBuffer));

        IFC(CD3D10VertexBuffer::Create(pD3DBuffer, &m_pVertexBuffers[i]));     

        ReleaseObject(pD3DBuffer);
    }

    IFC(CGeometryTesselationSink::Create(this, (IVertexBuffer**)m_pVertexBuffers, ARRAYSIZE(m_pVertexBuffers), &pTesselationSink));

    IFC(pTextureAtlasPool->GetOrCreateFirstTextureAtlas(&pFirstTextureAtlas));

    IFC(CRenderTargetBase::Initialize(pTesselationSink, pTextureAtlasPool, (CTextureAtlasView*)pFirstTextureAtlas->GetWhitePixelTexture()));

    IFC(m_pDevice->CreateVertexShader(g_vs_4_0, ARRAYSIZE(g_vs_4_0), &m_pVertexShader));

    IFC(m_pDevice->CreatePixelShader(g_ps_4_0, ARRAYSIZE(g_ps_4_0), &m_pPixelShader));

    {
        D3D10_INPUT_ELEMENT_DESC renderVertexDescription[] =
        {
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, RENDERVERTEX_POSITION_OFFSET, D3D10_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, RENDERVERTEX_COLOR_OFFSET, D3D10_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT, 0, RENDERVERTEX_TEXCOORDS_OFFSET, D3D10_INPUT_PER_VERTEX_DATA, 0 }
        };

        IFC(m_pDevice->CreateInputLayout(renderVertexDescription, ARRAYSIZE(renderVertexDescription), g_vs_4_0, ARRAYSIZE(g_vs_4_0), &m_pInputLayout));
    }

    {
        D3D10_BUFFER_DESC transformBuffer = { };

        transformBuffer.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
        transformBuffer.ByteWidth = sizeof(FLOAT) * (4 * 4);
        transformBuffer.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
        transformBuffer.Usage = D3D10_USAGE_DYNAMIC;
        transformBuffer.MiscFlags = 0;

        IFC(m_pDevice->CreateBuffer(&transformBuffer, NULL, &m_pTransformBuffer));
    }

    {
        D3D10_RASTERIZER_DESC rasterizerDescription = { };

        rasterizerDescription.FillMode = D3D10_FILL_SOLID;
        rasterizerDescription.CullMode = D3D10_CULL_NONE;
        rasterizerDescription.FrontCounterClockwise = FALSE;
        rasterizerDescription.DepthBias = 0;
        rasterizerDescription.DepthBiasClamp = 0;
        rasterizerDescription.SlopeScaledDepthBias = 0;
        rasterizerDescription.DepthClipEnable = TRUE;
        rasterizerDescription.ScissorEnable = FALSE;
        rasterizerDescription.MultisampleEnable = FALSE;
        rasterizerDescription.AntialiasedLineEnable = FALSE;

        IFC(m_pDevice->CreateRasterizerState(&rasterizerDescription, &m_pRasterizerState));
    }

    {
        D3D10_SAMPLER_DESC samplerDescription = { };

        samplerDescription.Filter = D3D10_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDescription.AddressU = D3D10_TEXTURE_ADDRESS_CLAMP;
        samplerDescription.AddressV = D3D10_TEXTURE_ADDRESS_CLAMP;
        samplerDescription.AddressW = D3D10_TEXTURE_ADDRESS_CLAMP;
        samplerDescription.ComparisonFunc = D3D10_COMPARISON_NEVER;
        samplerDescription.MinLOD = 0;
        samplerDescription.MaxLOD = D3D10_FLOAT32_MAX;

        IFC(m_pDevice->CreateSamplerState(&samplerDescription, &m_pSamplerState));
    }

Cleanup:
    ReleaseObject(pD3DBuffer);
    ReleaseObject(pTesselationSink);
    ReleaseObject(pFirstTextureAtlas);

    return hr;
}

__override __checkReturn HRESULT 
CD3D10RenderTarget::BeginRendering(
    )
{
    HRESULT hr = S_OK;
    FLOAT projectionMatrix[16] = { };
    D3D10_VIEWPORT viewPort = { };
    SizeF viewSize;
    BYTE* pTransformData = NULL;

    IFC(CRenderTargetBase::BeginRendering());

    //
    // Initialize view port.
    //
    viewSize = GetSize();

    viewPort.Width = (UINT)viewSize.width;
    viewPort.Height = (UINT)viewSize.height;
    viewPort.MinDepth = 0.0f;
    viewPort.MaxDepth = 0.0f;
    viewPort.TopLeftX = 0;
    viewPort.TopLeftY = 0;

    m_pDevice->RSSetViewports(1, &viewPort);

    //
    // Set shaders.
    //
    m_pDevice->VSSetShader(m_pVertexShader);

    m_pDevice->PSSetShader(m_pPixelShader);

    //
    // Set input layout.
    //
    m_pDevice->IASetInputLayout(m_pInputLayout);

    //
    // Set up orthographic projection matrix.
    // 
    {
        FLOAT nearPlane = 0.001f;
        FLOAT farPlane = 1000.0f;

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
        projectionMatrix[10] = 1.0f / (farPlane - nearPlane);
        projectionMatrix[11] = 0.0f;

        projectionMatrix[12] = (left + right) / (left - right);
        projectionMatrix[13] = (top + bottom) / (bottom - top);
        projectionMatrix[14] = nearPlane / (nearPlane - farPlane);
        projectionMatrix[15] = 1.0f;
    }

    IFC(m_pTransformBuffer->Map(D3D10_MAP_WRITE_DISCARD, NULL, (void**)&pTransformData));

    memcpy(pTransformData, projectionMatrix, sizeof(projectionMatrix));

    m_pDevice->VSSetConstantBuffers(0, 1, &m_pTransformBuffer);

    //
    // Set rasterization state (disable culling etc.)
    //
    m_pDevice->RSSetState(m_pRasterizerState);

    //
    // Set sampler state.
    //
    m_pDevice->PSSetSamplers(0, 1, &m_pSamplerState);

    //
    // Set primitive rendering type.
    //
    m_pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

Cleanup:
    if (pTransformData != NULL)
    {
        m_pTransformBuffer->Unmap();
    }

    return hr;
}

__override __checkReturn HRESULT 
CD3D10RenderTarget::EndRendering(
    )
{
    HRESULT hr = S_OK;

    IFC(CRenderTargetBase::EndRendering());

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CD3D10RenderTarget::BindTexture(
    __in ITexture* pTexture
    )
{
    HRESULT hr = S_OK;
    ID3D10ShaderResourceView* pShaderResourceView = NULL;
    ID3D10ShaderResourceView* pNewResourceView = NULL;
    CStagingTextureWrapper* pTextureWrapper = NULL;
    CD3D10Texture* pD3DTexture = NULL;

    pTextureWrapper = (CStagingTextureWrapper*)pTexture;
    pD3DTexture = (CD3D10Texture*)pTextureWrapper->GetTargetTexture();

    pShaderResourceView = pD3DTexture->GetResourceView();

    if (pShaderResourceView == NULL)
    {
        D3D10_TEXTURE2D_DESC textureDescription = { };

        ID3D10Texture2D* pTexture2D = pD3DTexture->GetD3DTexture();

        pTexture2D->GetDesc(&textureDescription);

        D3D10_SHADER_RESOURCE_VIEW_DESC resourceViewDescription = { };

        resourceViewDescription.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
        resourceViewDescription.Format = textureDescription.Format;
        resourceViewDescription.Texture2D.MipLevels = textureDescription.MipLevels;
        resourceViewDescription.Texture2D.MostDetailedMip = textureDescription.MipLevels - 1;

        IFC(m_pDevice->CreateShaderResourceView(pTexture2D, &resourceViewDescription, &pNewResourceView));

        pD3DTexture->SetResourceView(pNewResourceView);

        pShaderResourceView = pNewResourceView;
    }

    m_pDevice->PSSetShaderResources(0, 1, &pShaderResourceView);

Cleanup:
    ReleaseObject(pNewResourceView);

    return hr;
}

__override __checkReturn HRESULT 
CD3D10RenderTarget::OnTesselatedGeometryBatch(
    __in IVertexBuffer* pVertexBuffer
    )
{
    HRESULT hr = S_OK;
    CD3D10VertexBuffer* pD3DVertexBuffer = NULL;
    
    pD3DVertexBuffer = (CD3D10VertexBuffer*)pVertexBuffer;

    {
        ID3D10Buffer* pBuffer = pD3DVertexBuffer->GetD3DBuffer();
        const UINT32 stride = sizeof(RenderVertex);
        const UINT32 offset = 0;

        m_pDevice->IASetVertexBuffers(0, 1, &pBuffer, &stride, &offset);
    }

    m_pDevice->Draw(pVertexBuffer->GetVertexCount(), 0);

    return hr;
}

__override __checkReturn HRESULT 
CD3D10RenderTarget::Clear( 
    const ColorF& Color 
    )
{
    HRESULT hr = S_OK;
    FLOAT clearColor[] = { Color.r, Color.g, Color.b, Color.a };

    m_pDevice->ClearRenderTargetView(m_pRenderTargetView, clearColor);

    return hr;
}