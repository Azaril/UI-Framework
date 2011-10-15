#include "D3D9GraphicsDevice.h"
#include "CoreGeometryProvider.h"
#include "WICImagingProvider.h"
#include "FreetypeTextProvider.h"
#include "D3D9Texture.h"

typedef IDirect3D9* (WINAPI *Direct3DCreate9Func)(
    UINT SDKVersion
    );

CD3D9GraphicsDevice::CD3D9GraphicsDevice(
    )
    : m_pTextProvider(NULL)
    , m_pImagingProvider(NULL)
    , m_pGeometryProvider(NULL)
    , m_D3D9Module(NULL)
    , m_pD3D(NULL)
    , m_pDevice(NULL)
    , m_FocusWindow(NULL)
    , m_pTextureAtlasPool(NULL)
{
}

CD3D9GraphicsDevice::~CD3D9GraphicsDevice(
    )
{
    ReleaseObject(m_pTextProvider);
    ReleaseObject(m_pImagingProvider);
    ReleaseObject(m_pGeometryProvider);

    ReleaseObject(m_pTextureAtlasPool);
    
    ReleaseObject(m_pD3D);
    ReleaseObject(m_pDevice);

    if (m_D3D9Module != NULL)
    {
        FreeLibrary(m_D3D9Module);
    }
}

__checkReturn HRESULT
CD3D9GraphicsDevice::Initialize(
    HWND focusWindow
    )
{
    HRESULT hr = S_OK;
    Direct3DCreate9Func CreateFunc = NULL;
    D3DCAPS9 deviceCapabilites = { };

    m_FocusWindow = focusWindow;

    m_D3D9Module = LoadLibrary(L"D3D9.dll");
    IFCEXPECT(m_D3D9Module != NULL);

    CreateFunc = (Direct3DCreate9Func)GetProcAddress(m_D3D9Module, "Direct3DCreate9");
    IFCEXPECT(CreateFunc != NULL);

    m_pD3D = CreateFunc(D3D_SDK_VERSION);
    IFCPTR(m_pD3D);

    {
        D3DPRESENT_PARAMETERS presentParameters = { };

        presentParameters.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
        presentParameters.EnableAutoDepthStencil = FALSE;

        presentParameters.BackBufferCount = 0;
        presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;

        presentParameters.BackBufferHeight = 0;
        presentParameters.BackBufferWidth = 0;

        presentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
        presentParameters.MultiSampleQuality = 0;

        presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;

        presentParameters.hDeviceWindow = m_FocusWindow;
        presentParameters.Windowed = TRUE;

        presentParameters.Flags = 0;
        presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

        if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, NULL, D3DCREATE_HARDWARE_VERTEXPROCESSING, &presentParameters, &m_pDevice)))
        {
            IFC(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, NULL, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &presentParameters, &m_pDevice));
        }
    }

    IFC(m_pDevice->GetDeviceCaps(&deviceCapabilites));

    IFC(CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >::Create(deviceCapabilites.MaxTextureWidth, deviceCapabilites.MaxTextureWidth, this, &m_pTextureAtlasPool));

    IFC(CreateTextProvider(&m_pTextProvider));

    IFC(CreateImagingProvider(&m_pImagingProvider));

    IFC(CreateGeometryProvider(&m_pGeometryProvider));

Cleanup:
    return hr;
}

__checkReturn HRESULT
CD3D9GraphicsDevice::CreateHWNDRenderTarget(
    HWND Window,
    __deref_out CD3D9HWNDRenderTarget** ppRenderTarget
    )
{
    HRESULT hr = S_OK;
    IDirect3DDevice9* pDevice = NULL;
    CD3D9HWNDRenderTarget* pHWNDRenderTarget = NULL;
    IDirect3DSwapChain9* pSwapChain = NULL;

    IFCEXPECT(Window != NULL);
    IFCPTR(ppRenderTarget);

    if (Window != m_FocusWindow)
    {
        D3DPRESENT_PARAMETERS presentParameters = { };

        presentParameters.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
        presentParameters.EnableAutoDepthStencil = FALSE;

        presentParameters.BackBufferCount = 0;
        presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;

        presentParameters.BackBufferHeight = 0;
        presentParameters.BackBufferWidth = 0;

        presentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
        presentParameters.MultiSampleQuality = 0;

        presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;

        presentParameters.hDeviceWindow = m_FocusWindow;
        presentParameters.Windowed = TRUE;

        presentParameters.Flags = 0;
        presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

        IFC(m_pDevice->CreateAdditionalSwapChain(&presentParameters, &pSwapChain));
    }
    else
    {
        IFC(m_pDevice->GetSwapChain(0, &pSwapChain));
    }

    IFC(CD3D9HWNDRenderTarget::Create(m_pDevice, pSwapChain, m_pTextureAtlasPool, &pHWNDRenderTarget));

    *ppRenderTarget = pHWNDRenderTarget;
    pHWNDRenderTarget = NULL;

Cleanup:
    ReleaseObject(pDevice);

    return hr;
}

__override __checkReturn HRESULT 
CD3D9GraphicsDevice::GetTextProvider(
    __deref_out CTextProvider** ppTextProvider
    )
{
    HRESULT hr = S_OK;

    IFCPTR(ppTextProvider);

    *ppTextProvider = m_pTextProvider;
    AddRefObject(m_pTextProvider);

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CD3D9GraphicsDevice::GetImagingProvider(
    CImagingProvider** ppImagingProvider
    )
{
    HRESULT hr = S_OK;

    IFCPTR(ppImagingProvider);

    *ppImagingProvider = m_pImagingProvider;
    AddRefObject(m_pImagingProvider);

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CD3D9GraphicsDevice::GetGeometryProvider(
    CGeometryProvider** ppGeometryProvider
    )
{
    HRESULT hr = S_OK;

    IFCPTR(ppGeometryProvider);

    *ppGeometryProvider = m_pGeometryProvider;
    AddRefObject(m_pGeometryProvider);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CD3D9GraphicsDevice::CreateTextProvider(
    __deref_out CTextProvider** ppTextProvider
    )
{
    HRESULT hr = S_OK;
    CFreetypeTextProvider* pFreetypeTextProvider = NULL;

    IFCPTR(ppTextProvider);

    if(SUCCEEDED(CFreetypeTextProvider::Create(m_pTextureAtlasPool, &pFreetypeTextProvider)))
    {
        *ppTextProvider = pFreetypeTextProvider;
        pFreetypeTextProvider = NULL;
        goto Cleanup;
    }

    IFC(E_FAIL);

Cleanup:
    ReleaseObject(pFreetypeTextProvider);

    return hr;
}

__checkReturn HRESULT 
CD3D9GraphicsDevice::CreateImagingProvider(
    __deref_out CImagingProvider** ppImagingProvider
    )
{
    HRESULT hr = S_OK;
    CWICImagingProvider* pWICImagingProvider = NULL;

    IFCPTR(ppImagingProvider);

    if(SUCCEEDED(CWICImagingProvider::Create(&pWICImagingProvider)))
    {
        *ppImagingProvider = pWICImagingProvider;
        pWICImagingProvider = NULL;
        goto Cleanup;
    }

    IFC(E_FAIL);

Cleanup:
    ReleaseObject(pWICImagingProvider);

    return hr;
}

__checkReturn HRESULT 
CD3D9GraphicsDevice::CreateGeometryProvider(
    __deref_out CGeometryProvider** ppGeometryProvider
    )
{
    HRESULT hr = S_OK;
    CCoreGeometryProvider* pCoreGeometryProvider = NULL;

    IFCPTR(ppGeometryProvider);

    if(SUCCEEDED(CCoreGeometryProvider::Create(&pCoreGeometryProvider)))
    {
        *ppGeometryProvider = pCoreGeometryProvider;
        pCoreGeometryProvider = NULL;
        goto Cleanup;
    }

    IFC(E_FAIL);

Cleanup:
    ReleaseObject(pCoreGeometryProvider);

    return hr;
}

__override __checkReturn HRESULT 
CD3D9GraphicsDevice::AllocateTexture(
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