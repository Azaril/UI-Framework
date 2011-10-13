#include "D3D10GraphicsDevice.h"
#include "CoreGeometryProvider.h"
#include "WICImagingProvider.h"

CD3D10GraphicsDevice::CD3D10GraphicsDevice(
    )
    : m_pTextProvider(NULL)
    , m_pImagingProvider(NULL)
    , m_pGeometryProvider(NULL)
    , m_D3D10Module(NULL)
    , m_pCreateDevice(NULL)
    , m_pCreateDeviceAndSwapChain(NULL)
{
}

CD3D10GraphicsDevice::~CD3D10GraphicsDevice(
    )
{
    ReleaseObject(m_pTextProvider);
    ReleaseObject(m_pImagingProvider);
    ReleaseObject(m_pGeometryProvider);

    if (m_D3D10Module != NULL)
    {
        FreeLibrary(m_D3D10Module);
    }
}

__checkReturn HRESULT
CD3D10GraphicsDevice::Initialize(
    )
{
    HRESULT hr = S_OK;

    m_D3D10Module = LoadLibrary(L"D3D10.dll");
    IFCEXPECT(m_D3D10Module != NULL);

    m_pCreateDevice = (D3D10CreateDeviceFunc)GetProcAddress(m_D3D10Module, "D3D10CreateDevice");
    IFCEXPECT(m_pCreateDevice != NULL);

    m_pCreateDeviceAndSwapChain = (D3D10CreateDeviceAndSwapChainFunc)GetProcAddress(m_D3D10Module, "D3D10CreateDeviceAndSwapChain");
    IFCEXPECT(m_pCreateDeviceAndSwapChain != NULL);

    IFC(CreateTextProvider(&m_pTextProvider));

    IFC(CreateImagingProvider(&m_pImagingProvider));

    IFC(CreateGeometryProvider(&m_pGeometryProvider));

Cleanup:
    return hr;
}

__checkReturn HRESULT
CD3D10GraphicsDevice::CreateHWNDRenderTarget(
    HWND Window,
    __deref_out CD3D10HWNDRenderTarget** ppRenderTarget
    )
{
    HRESULT hr = S_OK;
    ID3D10Device* pDevice = NULL;
    IDXGISwapChain* pSwapChain = NULL;
    CD3D10HWNDRenderTarget* pHWNDRenderTarget = NULL;

    IFCEXPECT(Window != NULL);
    IFCPTR(ppRenderTarget);

    {
        DXGI_SWAP_CHAIN_DESC swapChainDescription = { };

        swapChainDescription.BufferCount = 1;
        swapChainDescription.BufferDesc.Width = 0;
        swapChainDescription.BufferDesc.Height = 0;
        //TODO: Check what format to use here.
        swapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDescription.BufferDesc.RefreshRate.Numerator = 60;
        swapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDescription.OutputWindow = Window;
        swapChainDescription.SampleDesc.Count = 1;
        swapChainDescription.SampleDesc.Quality = 0;
        swapChainDescription.Windowed = TRUE;
        
        IFC(m_pCreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, D3D10_CREATE_DEVICE_SINGLETHREADED, D3D10_SDK_VERSION, &swapChainDescription, &pSwapChain, &pDevice));
    }

    IFC(CD3D10HWNDRenderTarget::Create(pDevice, pSwapChain, &pHWNDRenderTarget));

    *ppRenderTarget = pHWNDRenderTarget;
    pHWNDRenderTarget = NULL;

Cleanup:
    ReleaseObject(pDevice);

    return hr;
}

__override __checkReturn HRESULT 
CD3D10GraphicsDevice::GetTextProvider(
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
CD3D10GraphicsDevice::GetImagingProvider(
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
CD3D10GraphicsDevice::GetGeometryProvider(
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
CD3D10GraphicsDevice::CreateTextProvider(
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
CD3D10GraphicsDevice::CreateImagingProvider(
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
CD3D10GraphicsDevice::CreateGeometryProvider(
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