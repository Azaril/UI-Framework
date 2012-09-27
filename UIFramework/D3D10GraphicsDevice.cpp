#include "D3D10GraphicsDevice.h"
#include "CoreGeometryProvider.h"
#include "WICImagingProvider.h"
#include "D3D10Texture.h"
#include "StagingTextureWrapper.h"
#include "FreetypeTextProvider.h"

#if defined(FRAMEWORK_D3D10)

CD3D10GraphicsDevice::CD3D10GraphicsDevice(
    )
    : m_pTextProvider(NULL)
    , m_pImagingProvider(NULL)
    , m_pGeometryProvider(NULL)
    , m_D3D10Module(NULL)
    , m_pCreateDevice(NULL)
    , m_DXGIModule(NULL)
    , m_pCreateDXGIFactory(NULL)
    , m_pDevice(NULL)
    , m_pStagingTextureAtlasPool(NULL)
    , m_pTextureAtlasPool(NULL)
    , m_pDXGIFactory(NULL)
{
}

CD3D10GraphicsDevice::~CD3D10GraphicsDevice(
    )
{
    ReleaseObject(m_pTextProvider);
    ReleaseObject(m_pImagingProvider);
    ReleaseObject(m_pGeometryProvider);

    ReleaseObject(m_pTextureAtlasPool);
    ReleaseObject(m_pStagingTextureAtlasPool);
    ReleaseObject(m_pDevice);

    ReleaseObject(m_pDXGIFactory);

    if (m_D3D10Module != NULL)
    {
        FreeLibrary(m_D3D10Module);
    }

    if (m_DXGIModule != NULL)
    {
        FreeLibrary(m_DXGIModule);
    }
}

__checkReturn HRESULT
CD3D10GraphicsDevice::Initialize(
    )
{
    HRESULT hr = S_OK;
    IDXGIAdapter* pAdapter = NULL;
    ID3D10Device* pDevice = NULL;

    m_D3D10Module = LoadLibrary(L"D3D10.dll");
    IFCEXPECT(m_D3D10Module != NULL);

    m_pCreateDevice = (D3D10CreateDeviceFunc)GetProcAddress(m_D3D10Module, "D3D10CreateDevice");
    IFCEXPECT(m_pCreateDevice != NULL);

    IFC(EnsureDXGIFactory());

    //TODO: Pick the correct adapter.
    IFC(m_pDXGIFactory->EnumAdapters(0, &pAdapter));

    IFC(m_pCreateDevice(pAdapter, D3D10_DRIVER_TYPE_HARDWARE, NULL, D3D10_CREATE_DEVICE_SINGLETHREADED, D3D10_SDK_VERSION, &pDevice));

    IFC(Initialize(pDevice));

Cleanup:
    ReleaseObject(pAdapter);
    ReleaseObject(pDevice);

    return hr;
}

__checkReturn HRESULT
CD3D10GraphicsDevice::Initialize(
    __in ID3D10Device* pDevice
    )
{
    HRESULT hr = S_OK;

    SetObject(m_pDevice, pDevice);

    {
        D3D10_TEXTURE2D_DESC textureDescription = { };

        textureDescription.MipLevels = 1;
        textureDescription.ArraySize = 1;
        textureDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDescription.SampleDesc.Count = 1;
        textureDescription.SampleDesc.Quality = 0;
        textureDescription.Usage = D3D10_USAGE_STAGING;
        textureDescription.BindFlags = 0;
        textureDescription.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
        textureDescription.MiscFlags = 0;

        m_StagingTextureAllocator.Initialize(this, textureDescription);

        //TODO: This need to be changed so that the atlas pool will check if the textures are in use, currently causes pipeline stalls
        //      as textures that are currently being copied are trying to be mapped.
        //TODO: Dynamically size this.
        IFC(CTextureAtlasPool< StagingTextureAtlasType >::Create(4096, 4096, &m_StagingTextureAllocator, &m_pStagingTextureAtlasPool));
    }

    //TODO: Dynamically size this.
    IFC(CTextureAtlasPool< RenderTextureAtlasType >::Create(4096, 4096, this, &m_pTextureAtlasPool));

    IFC(CreateTextProvider(&m_pTextProvider));

    IFC(CreateImagingProvider(&m_pImagingProvider));

    IFC(CreateGeometryProvider(&m_pGeometryProvider));

Cleanup:
    return hr;
}

__checkReturn HRESULT
CD3D10GraphicsDevice::EnsureDXGIFactory(
    )
{
    HRESULT hr = S_OK;

    if (m_DXGIModule == NULL)
    {
        m_DXGIModule = LoadLibrary(L"DXGI.dll");
        IFCEXPECT(m_DXGIModule != NULL);
    }

    if (m_pCreateDXGIFactory == NULL)
    {
        m_pCreateDXGIFactory = (CreateDXGIFactoryFunc)GetProcAddress(m_DXGIModule, "CreateDXGIFactory");
        IFCEXPECT(m_pCreateDXGIFactory != NULL);
    }

    if (m_pDXGIFactory == NULL)
    {
        IFC(m_pCreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_pDXGIFactory));
    }

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
    IDXGISwapChain* pSwapChain = NULL;
    CD3D10HWNDRenderTarget* pHWNDRenderTarget = NULL;

    IFCEXPECT(Window != NULL);
    IFCPTR(ppRenderTarget);

    IFC(EnsureDXGIFactory());

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

        IFC(m_pDXGIFactory->CreateSwapChain(m_pDevice, &swapChainDescription, &pSwapChain));
    }

    IFC(CD3D10HWNDRenderTarget::Create(m_pDevice, pSwapChain, m_pTextureAtlasPool, &pHWNDRenderTarget));

    *ppRenderTarget = pHWNDRenderTarget;
    pHWNDRenderTarget = NULL;

Cleanup:
    ReleaseObject(pSwapChain);

    return hr;
}

__checkReturn HRESULT
CD3D10GraphicsDevice::CreateSurfaceRenderTarget(
    __in ID3D10Texture2D* pRenderTarget,
    __deref_out CD3D10SurfaceRenderTarget** ppRenderTarget
    )
{
    HRESULT hr = S_OK;
    CD3D10SurfaceRenderTarget* pSurfaceRenderTarget = NULL;

    IFC(CD3D10SurfaceRenderTarget::Create(m_pDevice, pRenderTarget, m_pTextureAtlasPool, &pSurfaceRenderTarget));

    *ppRenderTarget = pSurfaceRenderTarget;
    pSurfaceRenderTarget = NULL;

Cleanup:
    ReleaseObject(pSurfaceRenderTarget);

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
CD3D10GraphicsDevice::CreateImagingProvider(
    __deref_out CImagingProvider** ppImagingProvider
    )
{
    HRESULT hr = S_OK;

#if defined(FRAMEWORK_WIC)
    CWICImagingProvider* pWICImagingProvider = NULL;
#endif

    IFCPTR(ppImagingProvider);

#if defined(FRAMEWORK_WIC)
    if(SUCCEEDED(CWICImagingProvider::Create(&pWICImagingProvider)))
    {
        *ppImagingProvider = pWICImagingProvider;
        pWICImagingProvider = NULL;
        goto Cleanup;
    }
#endif

    IFC(E_FAIL);

Cleanup:
#if defined(FRAMEWORK_WIC)
    ReleaseObject(pWICImagingProvider);
#endif

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

__checkReturn HRESULT 
CD3D10GraphicsDevice::AllocateTexture(
    UINT32 Width,
    UINT32 Height,
    __deref_out ITexture** ppTexture
    )
{
    HRESULT hr = S_OK;
    CStagingTextureWrapper* pStagingTexture = NULL;

    IFC(AllocateTexture(Width, Height, &pStagingTexture));

    *ppTexture = pStagingTexture;
    pStagingTexture = NULL;

Cleanup:
    ReleaseObject(pStagingTexture);

    return hr;
}

__checkReturn HRESULT 
CD3D10GraphicsDevice::AllocateTexture(
    UINT32 Width,
    UINT32 Height,
    __deref_out IBatchUpdateTexture** ppTexture
    )
{
    HRESULT hr = S_OK;
    CStagingTextureWrapper* pStagingTexture = NULL;

    IFC(AllocateTexture(Width, Height, &pStagingTexture));

    *ppTexture = pStagingTexture;
    pStagingTexture = NULL;

Cleanup:
    ReleaseObject(pStagingTexture);

    return hr;
}

__checkReturn HRESULT 
CD3D10GraphicsDevice::AllocateTexture(
    UINT32 Width,
    UINT32 Height,
    __deref_out CStagingTextureWrapper** ppTexture
    )
{
    HRESULT hr = S_OK;
    CD3D10Texture* pTexture = NULL;
    CStagingTextureWrapper* pStagingTexture = NULL;

    //
    // Create rendering texture.
    //
    {
        D3D10_TEXTURE2D_DESC textureDescription = { };

        textureDescription.Width = Width;
        textureDescription.Height = Height;
        textureDescription.MipLevels = 1;
        textureDescription.ArraySize = 1;
        textureDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDescription.SampleDesc.Count = 1;
        textureDescription.SampleDesc.Quality = 0;
        textureDescription.Usage = D3D10_USAGE_DEFAULT;
        textureDescription.BindFlags = D3D10_BIND_SHADER_RESOURCE;
        textureDescription.CPUAccessFlags = 0;
        textureDescription.MiscFlags = 0;

        IFC(AllocateTexture(textureDescription, &pTexture));
    }

    //
    // Create staging buffer texture wrapper.
    //
    IFC(CStagingTextureWrapper::Create(m_pStagingTextureAtlasPool, this, pTexture, &pStagingTexture));

    *ppTexture = pStagingTexture;
    pStagingTexture = NULL;

Cleanup:
    ReleaseObject(pTexture);

    return hr;
}

__checkReturn HRESULT 
CD3D10GraphicsDevice::AllocateTexture(
    const D3D10_TEXTURE2D_DESC& textureDescription,
    __deref_out CD3D10Texture** ppTexture
    )
{
    HRESULT hr = S_OK;
    ID3D10Texture2D* pD3DTexture = NULL;
    CD3D10Texture* pTexture = NULL;

    IFC(m_pDevice->CreateTexture2D(&textureDescription, NULL, &pD3DTexture));

    IFC(CD3D10Texture::Create(pD3DTexture, &pTexture));

    *ppTexture = pTexture;
    pTexture = NULL;

Cleanup:
    ReleaseObject(pTexture);
    ReleaseObject(pD3DTexture);

    return hr;
}

__override __checkReturn HRESULT 
CD3D10GraphicsDevice::AddUpdate(
    __in ITexture* pSource,
    const RectU& sourceRect,
    __in ITexture* pDestination,
    const Point2U& destOffset
    )
{
    HRESULT hr = S_OK;
    CTextureAtlasView* pSourceView = NULL;
    CD3D10Texture* pSourceTexture = NULL;
    CD3D10Texture* pDestinationTexture = NULL;

    pSourceView = (CTextureAtlasView*)pSource;

    pSourceTexture = (CD3D10Texture*)pSourceView->GetTexture();
    pDestinationTexture = (CD3D10Texture*)pDestination;

    {
        const RectU& viewSourceRect = pSourceView->GetRect();

        D3D10_BOX sourceBox = { };

        sourceBox.left = viewSourceRect.left + sourceRect.left;
        sourceBox.top = viewSourceRect.top + sourceRect.top;
        sourceBox.right = viewSourceRect.left + sourceRect.right;
        sourceBox.bottom = viewSourceRect.top + sourceRect.bottom;
        sourceBox.front = 0;
        sourceBox.back = 1;

        //TODO: Batch texture updates.
        m_pDevice->CopySubresourceRegion(pDestinationTexture->GetD3DTexture(), D3D10CalcSubresource(0, 0, 1), destOffset.x, destOffset.y, 0, pSourceTexture->GetD3DTexture(), D3D10CalcSubresource(0, 0, 1), &sourceBox);
    }

    return hr;
}

CD3D10GraphicsDevice::CRenderTextureAllocator::CRenderTextureAllocator(
    )
    : m_pGraphicsDevice(NULL)
{
    ZeroMemory(&m_TextureDescription, sizeof(m_TextureDescription));
}

void
CD3D10GraphicsDevice::CRenderTextureAllocator::Initialize(
    __in CD3D10GraphicsDevice* pGraphicsDevice,
    const D3D10_TEXTURE2D_DESC& baseDescription
    )
{
    m_pGraphicsDevice = pGraphicsDevice;
    m_TextureDescription = baseDescription;
}

__override __checkReturn HRESULT 
CD3D10GraphicsDevice::CRenderTextureAllocator::AllocateTexture(
    UINT32 Width,
    UINT32 Height,
    __deref_out ITexture** ppTexture
    )
{
    HRESULT hr = S_OK;
    D3D10_TEXTURE2D_DESC textureDescription = m_TextureDescription;
    CD3D10Texture* pNewTexture = NULL;

    textureDescription.Width = Width;
    textureDescription.Height = Height;

    IFC(m_pGraphicsDevice->AllocateTexture(textureDescription, &pNewTexture));

    *ppTexture = pNewTexture;
    pNewTexture = NULL;

Cleanup:
    ReleaseObject(pNewTexture);

    return hr;
}

__override __checkReturn HRESULT 
CD3D10GraphicsDevice::CRenderTextureAllocator::AllocateTexture(
    UINT32 Width,
    UINT32 Height,
    __deref_out IBatchUpdateTexture** ppTexture
    )
{
    HRESULT hr = S_OK;
    D3D10_TEXTURE2D_DESC textureDescription = m_TextureDescription;
    CD3D10Texture* pNewTexture = NULL;

    textureDescription.Width = Width;
    textureDescription.Height = Height;

    IFC(m_pGraphicsDevice->AllocateTexture(textureDescription, &pNewTexture));

    *ppTexture = pNewTexture;
    pNewTexture = NULL;

Cleanup:
    ReleaseObject(pNewTexture);

    return hr;
}

#endif