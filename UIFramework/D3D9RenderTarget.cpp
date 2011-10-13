#include "D3D9RenderTarget.h"
#include "D3D9Texture.h"
#include "TextureAtlasWithWhitePixel.h"

#include "Shaders/Direct3DHLSL/headers/ps_2_0.h"
#include "Shaders/Direct3DHLSL/headers/ps_3_0.h"
#include "Shaders/Direct3DHLSL/headers/ps_4_0.h"

#include "Shaders/Direct3DHLSL/headers/vs_2_0.h"
#include "Shaders/Direct3DHLSL/headers/vs_3_0.h"
#include "Shaders/Direct3DHLSL/headers/vs_4_0.h"

CD3D9RenderTarget::CD3D9RenderTarget(
    )
    : m_pDevice(NULL)
    , m_pVertexDeclaration(NULL)
    , m_pPixelShader(NULL)
    , m_pVertexShader(NULL)
{
    for (UINT32 i = 0; i < ARRAYSIZE(m_pVertexBuffers); ++i)
    {
        m_pVertexBuffers[i] = NULL;
    }
}

CD3D9RenderTarget::~CD3D9RenderTarget( 
    )
{
    for (UINT32 i = 0; i < ARRAYSIZE(m_pVertexBuffers); ++i)
    {
        ReleaseObject(m_pVertexBuffers[i]);
    }

    ReleaseObject(m_pPixelShader);
    ReleaseObject(m_pVertexShader);

    ReleaseObject(m_pVertexDeclaration);

    ReleaseObject(m_pDevice);
}

__checkReturn HRESULT 
CD3D9RenderTarget::Initialize(
    __in IDirect3DDevice9* pDevice
    )
{
    HRESULT hr = S_OK;
    IDirect3DVertexBuffer9* pD3DVertexBuffer = NULL;
    CGeometryTesselationSink* pTesselationSink = NULL;
    CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >* pTextureAtlasPool = NULL;
    CTextureAtlasWithWhitePixel< 1 >* pFirstTextureAtlas = NULL;
    D3DCAPS9 deviceCapabilites = { };

    SetObject(m_pDevice, pDevice);

    for (UINT32 i = 0; i < ARRAYSIZE(m_pVertexBuffers); ++i)
    {
        IFC(m_pDevice->CreateVertexBuffer(32768, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &pD3DVertexBuffer, NULL));

        IFC(CD3D9VertexBuffer::Create(pD3DVertexBuffer, &m_pVertexBuffers[i]));     

        ReleaseObject(pD3DVertexBuffer);
    }

    IFC(CGeometryTesselationSink::Create(this, (IVertexBuffer**)m_pVertexBuffers, ARRAYSIZE(m_pVertexBuffers), &pTesselationSink));

    IFC(pDevice->GetDeviceCaps(&deviceCapabilites));

    IFC(CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >::Create(deviceCapabilites.MaxTextureWidth, deviceCapabilites.MaxTextureWidth, this, &pTextureAtlasPool));

    IFC(pTextureAtlasPool->GetOrCreateFirstTextureAtlas(&pFirstTextureAtlas));

    IFC(CRenderTargetBase::Initialize(pTesselationSink, pTextureAtlasPool, (CTextureAtlasView*)pFirstTextureAtlas->GetWhitePixelTexture()));

    {
        const D3DVERTEXELEMENT9 renderVertexDescription[] = 
        {
            { 0, RENDERVERTEX_POSITION_OFFSET, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0  },
            { 0, RENDERVERTEX_COLOR_OFFSET, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
            { 0, RENDERVERTEX_TEXCOORDS_OFFSET, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
            D3DDECL_END()
        };

        IFC(m_pDevice->CreateVertexDeclaration(renderVertexDescription, &m_pVertexDeclaration));
    }

    if (deviceCapabilites.PixelShaderVersion >= D3DPS_VERSION(4, 0))
    {
        IFC(m_pDevice->CreatePixelShader((DWORD*)g_ps_4_0, &m_pPixelShader));
    }
    else if(deviceCapabilites.PixelShaderVersion >= D3DPS_VERSION(3, 0))
    {
        IFC(m_pDevice->CreatePixelShader((DWORD*)g_ps_3_0, &m_pPixelShader));
    }
    else if(deviceCapabilites.PixelShaderVersion >= D3DPS_VERSION(2, 0))
    {
        IFC(m_pDevice->CreatePixelShader((DWORD*)g_ps_3_0, &m_pPixelShader));
    }
    else
    {
        IFC(E_UNEXPECTED);
    }

    if (deviceCapabilites.VertexShaderVersion >= D3DVS_VERSION(4, 0))
    {
        IFC(m_pDevice->CreateVertexShader((DWORD*)g_vs_4_0, &m_pVertexShader));
    }
    else if(deviceCapabilites.VertexShaderVersion >= D3DVS_VERSION(3, 0))
    {
        IFC(m_pDevice->CreateVertexShader((DWORD*)g_vs_3_0, &m_pVertexShader));
    }
    else if(deviceCapabilites.VertexShaderVersion >= D3DVS_VERSION(2, 0))
    {
        IFC(m_pDevice->CreateVertexShader((DWORD*)g_vs_3_0, &m_pVertexShader));
    }
    else
    {
        IFC(E_UNEXPECTED);
    }

Cleanup:
    ReleaseObject(pD3DVertexBuffer);
    ReleaseObject(pTesselationSink);
    ReleaseObject(pTextureAtlasPool);
    ReleaseObject(pFirstTextureAtlas);

    return hr;
}

__override __checkReturn HRESULT 
CD3D9RenderTarget::BeginRendering(
    )
{
    HRESULT hr = S_OK;
    FLOAT projectionMatrix[16] = { };

    IFC(CRenderTargetBase::BeginRendering());

    IFC(m_pDevice->SetVertexDeclaration(m_pVertexDeclaration));
    IFC(m_pDevice->SetVertexShader(m_pVertexShader));
    IFC(m_pDevice->SetPixelShader(m_pPixelShader));

    //
    // Set up orthographic projection matrix.
    // 
    {
        SizeF size = GetSize();

        FLOAT nearPlane = 0.001f;
        FLOAT farPlane = 1000.0f;

        FLOAT left = 0.0f;
        FLOAT right = size.width;
        FLOAT top = 0.0f;
        FLOAT bottom = size.height;

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

    IFC(m_pDevice->SetVertexShaderConstantF(0, projectionMatrix, 4));

    IFC(m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE));

    IFC(m_pDevice->BeginScene());

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CD3D9RenderTarget::EndRendering(
    )
{
    HRESULT hr = S_OK;

    IFC(CRenderTargetBase::EndRendering());

    IFC(m_pDevice->EndScene());

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CD3D9RenderTarget::BindTexture(
    __in ITexture* pTexture
    )
{
    HRESULT hr = S_OK;
    CD3D9Texture* pD3DTexture = NULL;

    pD3DTexture = (CD3D9Texture*)pTexture;

    IFC(m_pDevice->SetTexture(0, pD3DTexture->GetD3DTexture()));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CD3D9RenderTarget::OnTesselatedGeometryBatch(
    __in IVertexBuffer* pVertexBuffer
    )
{
    HRESULT hr = S_OK;
    CD3D9VertexBuffer* pD3DVertexBuffer = NULL;
    
    pD3DVertexBuffer = (CD3D9VertexBuffer*)pVertexBuffer;

    IFC(m_pDevice->SetStreamSource(0, pD3DVertexBuffer->GetD3DVertexBuffer(), 0, sizeof(RenderVertex)));

    IFC(m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, pVertexBuffer->GetVertexCount() / 3));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CD3D9RenderTarget::AllocateTexture(
    UINT32 Width,
    UINT32 Height,
    __deref_out ITexture** ppTexture
    )
{
    HRESULT hr = S_OK;
    IDirect3DTexture9* pD3DTexture = NULL;
    CD3D9Texture* pTexture = NULL;

    IFC(m_pDevice->CreateTexture(Width, Height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pD3DTexture, NULL));

    IFC(CD3D9Texture::Create(pD3DTexture, &pTexture));

    *ppTexture = pTexture;
    pTexture = NULL;

Cleanup:
    ReleaseObject(pTexture);
    ReleaseObject(pD3DTexture);

    return hr;
}

__override __checkReturn HRESULT 
CD3D9RenderTarget::Clear( 
    const ColorF& Color 
    )
{
    HRESULT hr = S_OK;

    IFC(m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_COLORVALUE(Color.r, Color.g, Color.g, Color.a), 1.0f, 0));

Cleanup:
    return hr;
}