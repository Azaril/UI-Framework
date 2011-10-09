#include "D3D9GraphicsDevice.h"
#include "CoreGeometryProvider.h"

typedef HRESULT (WINAPI *D2D1CreateFactoryFunc)( 
    __in D2D1_FACTORY_TYPE factoryType, 
    __in REFIID riid, 
    __in_opt CONST D2D1_FACTORY_OPTIONS *pFactoryOptions, 
    __out void **ppIFactory 
    );

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
{
}

CD3D9GraphicsDevice::~CD3D9GraphicsDevice(
    )
{
    ReleaseObject(m_pTextProvider);
    ReleaseObject(m_pImagingProvider);
    ReleaseObject(m_pGeometryProvider);
    
    ReleaseObject(m_pD3D);

    if (m_D3D9Module != NULL)
    {
        FreeLibrary(m_D3D9Module);
    }
}

__checkReturn HRESULT
CD3D9GraphicsDevice::Initialize(
    )
{
    HRESULT hr = S_OK;
    Direct3DCreate9Func CreateFunc = NULL;

    m_D3D9Module = LoadLibrary(L"D3D9.dll");
    IFCEXPECT(m_D3D9Module != NULL);

    CreateFunc = (Direct3DCreate9Func)GetProcAddress(m_D3D9Module, "Direct3DCreate9");
    IFCEXPECT(CreateFunc != NULL);

    m_pD3D = CreateFunc(D3D_SDK_VERSION);
    IFCPTR(m_pD3D);

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

    IFCEXPECT(Window != NULL);
    IFCPTR(ppRenderTarget);

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

        presentParameters.hDeviceWindow = Window;
        presentParameters.Windowed = TRUE;

        presentParameters.Flags = 0;
        presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

        if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Window, D3DCREATE_HARDWARE_VERTEXPROCESSING, &presentParameters, &pDevice)))
        {
            IFC(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Window, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &presentParameters, &pDevice));
        }
    }

    IFC(CD3D9HWNDRenderTarget::Create(pDevice, &pHWNDRenderTarget));

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
    // CDirectWriteTextProvider* pDirectWriteTextProvider = NULL;

    IFCPTR(ppTextProvider);

    // if(SUCCEEDED(CDirectWriteTextProvider::Create(&pDirectWriteTextProvider)))
    // {
    //     *ppTextProvider = pDirectWriteTextProvider;
    //     pDirectWriteTextProvider = NULL;
    //     goto Cleanup;
    // }

    //IFC(E_FAIL);

Cleanup:
    // ReleaseObject(pDirectWriteTextProvider);

    return hr;
}

__checkReturn HRESULT 
CD3D9GraphicsDevice::CreateImagingProvider(
    __deref_out CImagingProvider** ppImagingProvider
    )
{
    HRESULT hr = S_OK;
    // CWICImagingProvider* pWICImagingProvider = NULL;

    IFCPTR(ppImagingProvider);

    // if(SUCCEEDED(CWICImagingProvider::Create(&pWICImagingProvider)))
    // {
    //     *ppImagingProvider = pWICImagingProvider;
    //     pWICImagingProvider = NULL;
    //     goto Cleanup;
    // }

    //IFC(E_FAIL);

Cleanup:
    // ReleaseObject(pWICImagingProvider);

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