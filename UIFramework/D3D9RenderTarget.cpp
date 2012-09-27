#include "D3D9RenderTarget.h"
#include "D3D9Texture.h"
#include "TextureAtlasWithWhitePixel.h"

#if defined(FRAMEWORK_D3D9)

#if defined(_XBOX)

#include "Shaders/Direct3DHLSL/headers/d3dx/ps_2_0.h"
#include "Shaders/Direct3DHLSL/headers/d3dx/ps_3_0.h"
#include "Shaders/Direct3DHLSL/headers/d3dx/ps_4_0.h"

#include "Shaders/Direct3DHLSL/headers/d3dx/vs_2_0.h"
#include "Shaders/Direct3DHLSL/headers/d3dx/vs_3_0.h"
#include "Shaders/Direct3DHLSL/headers/d3dx/vs_4_0.h"

#else

#include "Shaders/Direct3DHLSL/headers/d3d/ps_2_0.h"
#include "Shaders/Direct3DHLSL/headers/d3d/ps_3_0.h"
#include "Shaders/Direct3DHLSL/headers/d3d/ps_4_0.h"

#include "Shaders/Direct3DHLSL/headers/d3d/vs_2_0.h"
#include "Shaders/Direct3DHLSL/headers/d3d/vs_3_0.h"
#include "Shaders/Direct3DHLSL/headers/d3d/vs_4_0.h"

#endif

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
    __in IDirect3DDevice9* pDevice,
    __in CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >* pTextureAtlasPool
    )
{
    HRESULT hr = S_OK;
    IDirect3DVertexBuffer9* pD3DVertexBuffer = NULL;
    CGeometryTesselationSink* pTesselationSink = NULL;
    CTextureAtlasWithWhitePixel< 1 >* pFirstTextureAtlas = NULL;
    D3DCAPS9 deviceCapabilites = { };

    SetObject(m_pDevice, pDevice);

    for (UINT32 i = 0; i < ARRAYSIZE(m_pVertexBuffers); ++i)
    {
#if defined(_XBOX)
		IFC(m_pDevice->CreateVertexBuffer(32768, D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &pD3DVertexBuffer, NULL));
#else
        IFC(m_pDevice->CreateVertexBuffer(32768, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &pD3DVertexBuffer, NULL));
#endif

        IFC(CD3D9VertexBuffer::Create(pD3DVertexBuffer, &m_pVertexBuffers[i]));     

        ReleaseObject(pD3DVertexBuffer);
    }

    //
    // Translate vertices by 0.5f to correctly offset sampling.
    //
    {
        Matrix3X2F vertexTransform = Matrix3X2F::Translation(0.5f, 0.5f);

        IFC(CGeometryTesselationSink::Create(this, (IVertexBuffer**)m_pVertexBuffers, ARRAYSIZE(m_pVertexBuffers), &vertexTransform, &pTesselationSink));
    }

    IFC(pDevice->GetDeviceCaps(&deviceCapabilites));

    IFC(pTextureAtlasPool->GetOrCreateFirstTextureAtlas(&pFirstTextureAtlas));

    IFC(CRenderTargetBase::Initialize(pTesselationSink, pTextureAtlasPool, (CTextureAtlasView*)pFirstTextureAtlas->GetWhitePixelTexture()));

    {
        const D3DVERTEXELEMENT9 renderVertexDescription[] = 
        {
            { 0, RENDERVERTEX_POSITION_OFFSET, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0  },
            { 0, RENDERVERTEX_COLOR_OFFSET, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
            { 0, RENDERVERTEX_TEXCOORDS_OFFSET, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
            { 0, RENDERVERTEX_MASKCOORDS_OFFSET, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
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
        IFC(m_pDevice->CreatePixelShader((DWORD*)g_ps_2_0, &m_pPixelShader));
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
        IFC(m_pDevice->CreateVertexShader((DWORD*)g_vs_2_0, &m_pVertexShader));
    }
    else
    {
        IFC(E_UNEXPECTED);
    }

Cleanup:
    ReleaseObject(pD3DVertexBuffer);
    ReleaseObject(pTesselationSink);
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
	
	IFC(m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE));
	IFC(m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE));

    IFC(m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE));

    IFC(m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE));
    IFC(m_pDevice->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_SRCALPHA));
    IFC(m_pDevice->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CD3D9RenderTarget::EndRendering(
    )
{
    HRESULT hr = S_OK;

    IFC(CRenderTargetBase::EndRendering());

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
CD3D9RenderTarget::BindMask(
    __in ITexture* pTexture
    )
{
    HRESULT hr = S_OK;
    CD3D9Texture* pD3DTexture = NULL;

    pD3DTexture = (CD3D9Texture*)pTexture;

    IFC(m_pDevice->SetTexture(1, pD3DTexture->GetD3DTexture()));

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
CD3D9RenderTarget::Clear( 
    const ColorF& Color 
    )
{
    HRESULT hr = S_OK;

    IFC(m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_COLORVALUE(Color.r, Color.g, Color.g, Color.a), 1.0f, 0));

Cleanup:
    return hr;
}

#endif