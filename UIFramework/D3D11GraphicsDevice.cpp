#include "D3D11GraphicsDevice.h"
#include "CoreGeometryProvider.h"
#include "WICImagingProvider.h"
#include "D3D11Texture.h"
#include "StagingTextureWrapper.h"
#include "FreetypeTextProvider.h"

#if defined(FRAMEWORK_D3D11)

CD3D11GraphicsDevice::CD3D11GraphicsDevice(
    )
    : m_pTextProvider(NULL)
    , m_pImagingProvider(NULL)
    , m_pGeometryProvider(NULL)
    , m_D3D11Module(NULL)
    , m_pCreateDevice(NULL)
    , m_DXGIModule(NULL)
    , m_pCreateDXGIFactory(NULL)
    , m_pDevice(NULL)
    , m_pStagingTextureAtlasPool(NULL)
    , m_pTextureAtlasPool(NULL)
    , m_pDXGIFactory(NULL)
    , m_pImmediateContext(NULL)
{
}

CD3D11GraphicsDevice::~CD3D11GraphicsDevice(
    )
{
    ReleaseObject(m_pTextProvider);
    ReleaseObject(m_pImagingProvider);
    ReleaseObject(m_pGeometryProvider);

    ReleaseObject(m_pTextureAtlasPool);
    ReleaseObject(m_pStagingTextureAtlasPool);
    ReleaseObject(m_pImmediateContext);
    ReleaseObject(m_pDevice);

    ReleaseObject(m_pDXGIFactory);

    if (m_D3D11Module != NULL)
    {
        FreeLibrary(m_D3D11Module);
    }

    if (m_DXGIModule != NULL)
    {
        FreeLibrary(m_DXGIModule);
    }
}

__checkReturn HRESULT
CD3D11GraphicsDevice::Initialize(
    )
{
    HRESULT hr = S_OK;
    IDXGIAdapter* pAdapter = NULL;
    ID3D11DeviceContext* pImmediateContext = NULL;
    ID3D11Device* pDevice = NULL;
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_9_1;

    m_D3D11Module = LoadLibrary(L"D3D11.dll");
    IFCEXPECT(m_D3D11Module != NULL);

    m_pCreateDevice = (D3D11CreateDeviceFunc)GetProcAddress(m_D3D11Module, "D3D11CreateDevice");
    IFCEXPECT(m_pCreateDevice != NULL);

    IFC(EnsureDXGIFactory());

    //TODO: Pick the correct adapter.
    IFC(m_pDXGIFactory->EnumAdapters(0, &pAdapter));

    {
        D3D_FEATURE_LEVEL featureLevels[] =
        {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
            D3D_FEATURE_LEVEL_9_3,
            D3D_FEATURE_LEVEL_9_2,
            D3D_FEATURE_LEVEL_9_1
        };

        IFC(m_pCreateDevice(pAdapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, D3D11_CREATE_DEVICE_SINGLETHREADED, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &pDevice, &featureLevel, &pImmediateContext));
    }

    IFC(Initialize(pDevice));

Cleanup:
    ReleaseObject(pDevice);
    ReleaseObject(pImmediateContext);
    ReleaseObject(pAdapter);

    return hr;
}

__checkReturn HRESULT
CD3D11GraphicsDevice::Initialize(
    __in ID3D11Device* pDevice
    )
{
    HRESULT hr = S_OK;

    SetObject(m_pDevice, pDevice);

    m_pDevice->GetImmediateContext(&m_pImmediateContext);

    {
        D3D11_TEXTURE2D_DESC textureDescription = { };

        textureDescription.MipLevels = 1;
        textureDescription.ArraySize = 1;
        textureDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDescription.SampleDesc.Count = 1;
        textureDescription.SampleDesc.Quality = 0;
        textureDescription.Usage = D3D11_USAGE_STAGING;
        textureDescription.BindFlags = 0;
        textureDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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
CD3D11GraphicsDevice::EnsureDXGIFactory(
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
CD3D11GraphicsDevice::CreateHWNDRenderTarget(
    HWND Window,
    __deref_out CD3D11HWNDRenderTarget** ppRenderTarget
    )
{
    HRESULT hr = S_OK;
    IDXGISwapChain* pSwapChain = NULL;
    CD3D11HWNDRenderTarget* pHWNDRenderTarget = NULL;

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

    IFC(CD3D11HWNDRenderTarget::Create(m_pDevice, pSwapChain, m_pTextureAtlasPool, &pHWNDRenderTarget));

    *ppRenderTarget = pHWNDRenderTarget;
    pHWNDRenderTarget = NULL;

Cleanup:
    ReleaseObject(pSwapChain);
    ReleaseObject(pHWNDRenderTarget);

    return hr;
}

__checkReturn HRESULT
CD3D11GraphicsDevice::CreateSurfaceRenderTarget(
    __in ID3D11Texture2D* pRenderTarget,
    __deref_out CD3D11SurfaceRenderTarget** ppRenderTarget
    )
{
    HRESULT hr = S_OK;
    CD3D11SurfaceRenderTarget* pSurfaceRenderTarget = NULL;

    IFC(CD3D11SurfaceRenderTarget::Create(m_pDevice, pRenderTarget, m_pTextureAtlasPool, &pSurfaceRenderTarget));

    *ppRenderTarget = pSurfaceRenderTarget;
    pSurfaceRenderTarget = NULL;

Cleanup:
    ReleaseObject(pSurfaceRenderTarget);

    return hr;
}

__override __checkReturn HRESULT 
CD3D11GraphicsDevice::GetTextProvider(
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
CD3D11GraphicsDevice::GetImagingProvider(
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
CD3D11GraphicsDevice::GetGeometryProvider(
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
CD3D11GraphicsDevice::CreateTextProvider(
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
CD3D11GraphicsDevice::CreateImagingProvider(
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
CD3D11GraphicsDevice::CreateGeometryProvider(
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
CD3D11GraphicsDevice::AllocateTexture(
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
CD3D11GraphicsDevice::AllocateTexture(
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
CD3D11GraphicsDevice::AllocateTexture(
    UINT32 Width,
    UINT32 Height,
    __deref_out CStagingTextureWrapper** ppTexture
    )
{
    HRESULT hr = S_OK;
    CD3D11Texture* pTexture = NULL;
    CStagingTextureWrapper* pStagingTexture = NULL;

    //
    // Create rendering texture.
    //
    {
        D3D11_TEXTURE2D_DESC textureDescription = { };

        textureDescription.Width = Width;
        textureDescription.Height = Height;
        textureDescription.MipLevels = 1;
        textureDescription.ArraySize = 1;
        textureDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDescription.SampleDesc.Count = 1;
        textureDescription.SampleDesc.Quality = 0;
        textureDescription.Usage = D3D11_USAGE_DEFAULT;
        textureDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;
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
CD3D11GraphicsDevice::AllocateTexture(
    const D3D11_TEXTURE2D_DESC& textureDescription,
    __deref_out CD3D11Texture** ppTexture
    )
{
    HRESULT hr = S_OK;
    ID3D11Texture2D* pD3DTexture = NULL;
    CD3D11Texture* pTexture = NULL;

    IFC(m_pDevice->CreateTexture2D(&textureDescription, NULL, &pD3DTexture));

    IFC(CD3D11Texture::Create(m_pImmediateContext, pD3DTexture, &pTexture));

    *ppTexture = pTexture;
    pTexture = NULL;

Cleanup:
    ReleaseObject(pTexture);
    ReleaseObject(pD3DTexture);

    return hr;
}

__override __checkReturn HRESULT 
CD3D11GraphicsDevice::AddUpdate(
    __in ITexture* pSource,
    const RectU& sourceRect,
    __in ITexture* pDestination,
    const Point2U& destOffset
    )
{
    HRESULT hr = S_OK;
    CTextureAtlasView* pSourceView = NULL;
    CD3D11Texture* pSourceTexture = NULL;
    CD3D11Texture* pDestinationTexture = NULL;

    pSourceView = (CTextureAtlasView*)pSource;

    pSourceTexture = (CD3D11Texture*)pSourceView->GetTexture();
    pDestinationTexture = (CD3D11Texture*)pDestination;

    {
        const RectU& viewSourceRect = pSourceView->GetRect();

        D3D11_BOX sourceBox = { };

        sourceBox.left = viewSourceRect.left + sourceRect.left;
        sourceBox.top = viewSourceRect.top + sourceRect.top;
        sourceBox.right = viewSourceRect.left + sourceRect.right;
        sourceBox.bottom = viewSourceRect.top + sourceRect.bottom;
        sourceBox.front = 0;
        sourceBox.back = 1;

        //TODO: Batch texture updates.
        m_pImmediateContext->CopySubresourceRegion(pDestinationTexture->GetD3DTexture(), D3D11CalcSubresource(0, 0, 1), destOffset.x, destOffset.y, 0, pSourceTexture->GetD3DTexture(), D3D11CalcSubresource(0, 0, 1), &sourceBox);
    }

    return hr;
}

CD3D11GraphicsDevice::CRenderTextureAllocator::CRenderTextureAllocator(
    )
    : m_pGraphicsDevice(NULL)
{
    ZeroMemory(&m_TextureDescription, sizeof(m_TextureDescription));
}

void
CD3D11GraphicsDevice::CRenderTextureAllocator::Initialize(
    __in CD3D11GraphicsDevice* pGraphicsDevice,
    const D3D11_TEXTURE2D_DESC& baseDescription
    )
{
    m_pGraphicsDevice = pGraphicsDevice;
    m_TextureDescription = baseDescription;
}

__override __checkReturn HRESULT 
CD3D11GraphicsDevice::CRenderTextureAllocator::AllocateTexture(
    UINT32 Width,
    UINT32 Height,
    __deref_out ITexture** ppTexture
    )
{
    HRESULT hr = S_OK;
    D3D11_TEXTURE2D_DESC textureDescription = m_TextureDescription;
    CD3D11Texture* pNewTexture = NULL;

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
CD3D11GraphicsDevice::CRenderTextureAllocator::AllocateTexture(
    UINT32 Width,
    UINT32 Height,
    __deref_out IBatchUpdateTexture** ppTexture
    )
{
    HRESULT hr = S_OK;
    D3D11_TEXTURE2D_DESC textureDescription = m_TextureDescription;
    CD3D11Texture* pNewTexture = NULL;

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