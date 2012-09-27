#include "D3D9GraphicsDevice.h"
#include "CoreGeometryProvider.h"
#include "WICImagingProvider.h"
#include "FreetypeTextProvider.h"
#include "D3D9Texture.h"
#include "StagingTextureWrapper.h"

#if defined(FRAMEWORK_D3D9)

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
    , m_pStagingTextureAtlasPool(NULL)
{
}

CD3D9GraphicsDevice::~CD3D9GraphicsDevice(
    )
{
    ReleaseObject(m_pTextProvider);
    ReleaseObject(m_pImagingProvider);
    ReleaseObject(m_pGeometryProvider);

    ReleaseObject(m_pTextureAtlasPool);
    ReleaseObject(m_pStagingTextureAtlasPool);
    
    ReleaseObject(m_pD3D);
    ReleaseObject(m_pDevice);

    if (m_D3D9Module != NULL)
    {
        FreeLibrary(m_D3D9Module);
    }
}

__checkReturn HRESULT
CD3D9GraphicsDevice::Initialize(
    IDirect3DDevice9* pDevice
    )
{
    HRESULT hr = S_OK;
    D3DDEVICE_CREATION_PARAMETERS creationParams = { };

    SetObject(m_pDevice, pDevice);

    IFC(pDevice->GetDirect3D(&m_pD3D));

    IFC(pDevice->GetCreationParameters(&creationParams));

    m_FocusWindow = creationParams.hFocusWindow;

    IFC(InitializeCommon());

Cleanup:
    return hr;
}

#if !defined(_XBOX)

__checkReturn HRESULT
CD3D9GraphicsDevice::Initialize(
    HWND focusWindow
    )
{
    HRESULT hr = S_OK;
    Direct3DCreate9Func CreateFunc = NULL;

    m_FocusWindow = focusWindow;

    m_D3D9Module = LoadLibraryA("D3D9.dll");
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

    IFC(InitializeCommon());

Cleanup:
    return hr;
}

#endif

__checkReturn HRESULT
CD3D9GraphicsDevice::InitializeCommon(
    )
{
    HRESULT hr = S_OK;
    D3DCAPS9 deviceCapabilites = { };

    IFC(m_pDevice->GetDeviceCaps(&deviceCapabilites));

    m_StagingTextureAllocator.Initialize(this, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM);

    UINT32 textureWidth = deviceCapabilites.MaxTextureWidth;
    UINT32 textureHeight = deviceCapabilites.MaxTextureHeight;

#if defined(_XBOX)
    textureWidth = std::min(textureWidth, 256);
    textureHeight = std::min(textureHeight, 256);
#endif

    //TODO: This need to be changed so that the atlas pool will check if the textures are in use, currently causes pipeline stalls
    //      as textures that are currently being copied are trying to be mapped.
    //TODO: Dynamically size this.
    IFC(CTextureAtlasPool< StagingTextureAtlasType >::Create(textureWidth, textureHeight, &m_StagingTextureAllocator, &m_pStagingTextureAtlasPool));

	IFC(CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >::Create(textureWidth, textureHeight, this, &m_pTextureAtlasPool));

    IFC(CreateTextProvider(&m_pTextProvider));

    IFC(CreateImagingProvider(&m_pImagingProvider));

    IFC(CreateGeometryProvider(&m_pGeometryProvider));

Cleanup:
    return hr;
}

#if !defined(_XBOX)

__checkReturn HRESULT
CD3D9GraphicsDevice::CreateHWNDRenderTarget(
    HWND Window,
    __deref_out CD3D9HWNDRenderTarget** ppRenderTarget
    )
{
    HRESULT hr = S_OK;
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
    ReleaseObject(pHWNDRenderTarget);

    return hr;
}

#endif

__checkReturn HRESULT
CD3D9GraphicsDevice::CreateSurfaceRenderTarget(
    __in IDirect3DSurface9* pSurface,
    __deref_out CD3D9SurfaceRenderTarget** ppRenderTarget
    )
{
    HRESULT hr = S_OK;
    CD3D9SurfaceRenderTarget* pSurfaceRenderTarget = NULL;

    IFCPTR(pSurface);
    IFCPTR(ppRenderTarget);

    IFC(CD3D9SurfaceRenderTarget::Create(m_pDevice, pSurface, m_pTextureAtlasPool, &pSurfaceRenderTarget));

    *ppRenderTarget = pSurfaceRenderTarget;
    pSurfaceRenderTarget = NULL;

Cleanup:
    ReleaseObject(pSurfaceRenderTarget);

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

#if defined(FRAMEWORK_WIC)

    CWICImagingProvider* pWICImagingProvider = NULL;

    IFCPTR(ppImagingProvider);

    if(SUCCEEDED(CWICImagingProvider::Create(&pWICImagingProvider)))
    {
        *ppImagingProvider = pWICImagingProvider;
        pWICImagingProvider = NULL;
        goto Cleanup;
    }

#endif

	//TODO: wiarchbe: Enable imaging component...
    //IFC(E_FAIL);

//Cleanup:

#if defined(FRAMEWORK_WIC)
    ReleaseObject(pWICImagingProvider);
#endif

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

__checkReturn HRESULT 
CD3D9GraphicsDevice::AllocateTexture(
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
CD3D9GraphicsDevice::AllocateTexture(
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
CD3D9GraphicsDevice::AllocateTexture(
    UINT32 Width,
    UINT32 Height,
    __deref_out CStagingTextureWrapper** ppTexture
    )
{
    HRESULT hr = S_OK;
    CD3D9Texture* pTexture = NULL;
    CStagingTextureWrapper* pStagingTexture = NULL;

    //
    // Create rendering texture.
    //
#if defined(_XBOX)
	IFC(AllocateTexture(Width, Height, 1, D3DUSAGE_CPU_CACHED_MEMORY, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pTexture));	
#else
    IFC(AllocateTexture(Width, Height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pTexture));
#endif

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

__override __checkReturn HRESULT 
CD3D9GraphicsDevice::AllocateTexture(
    UINT32 Width,
    UINT32 Height,
    UINT32 Levels,
    DWORD Usage,
    D3DFORMAT Format,
    D3DPOOL Pool,
    __deref_out CD3D9Texture** ppTexture
    )
{
    HRESULT hr = S_OK;
    IDirect3DTexture9* pD3DTexture = NULL;
    CD3D9Texture* pTexture = NULL;

    IFC(m_pDevice->CreateTexture(Width, Height, Levels, Usage, Format, Pool, &pD3DTexture, NULL));

    IFC(CD3D9Texture::Create(pD3DTexture, &pTexture));

    *ppTexture = pTexture;
    pTexture = NULL;

Cleanup:
    ReleaseObject(pTexture);
    ReleaseObject(pD3DTexture);

    return hr;
}

__override __checkReturn HRESULT 
CD3D9GraphicsDevice::AddUpdate(
    __in ITexture* pSource,
    const RectU& sourceRect,
    __in ITexture* pDestination,
    const Point2U& destOffset
    )
{
    HRESULT hr = S_OK;
    CTextureAtlasView* pSourceView = NULL;
    CD3D9Texture* pSourceTexture = NULL;
    CD3D9Texture* pDestinationTexture = NULL;
    IDirect3DSurface9* pSourceSurface = NULL;
    IDirect3DSurface9* pDestinationSurface = NULL;

    pSourceView = (CTextureAtlasView*)pSource;

    pSourceTexture = (CD3D9Texture*)pSourceView->GetTexture();
    pDestinationTexture = (CD3D9Texture*)pDestination;

    IFC(pSourceTexture->GetD3DTexture()->GetSurfaceLevel(0, &pSourceSurface));
    IFC(pDestinationTexture->GetD3DTexture()->GetSurfaceLevel(0, &pDestinationSurface));

    {
        const RectU& viewSourceRect = pSourceView->GetRect();

        RECT region = { sourceRect.left, sourceRect.top, sourceRect.right, sourceRect.bottom };

        region.left = viewSourceRect.left + sourceRect.left;
        region.top = viewSourceRect.top + sourceRect.top;
        region.right = viewSourceRect.left + sourceRect.right;
        region.bottom = viewSourceRect.top + sourceRect.bottom;

        const POINT target = { destOffset.x, destOffset. y };

        //TODO: Batch texture updates.
        IFC(m_pDevice->UpdateSurface(pSourceSurface, &region, pDestinationSurface, &target));
    }

Cleanup:
    ReleaseObject(pSourceSurface);
    ReleaseObject(pDestinationSurface);

    return hr;
}

CD3D9GraphicsDevice::CRenderTextureAllocator::CRenderTextureAllocator(
    )
    : m_pGraphicsDevice(NULL)
    , m_levels(0)
    , m_usage(0)
    , m_format(D3DFMT_UNKNOWN)
    , m_pool(D3DPOOL_DEFAULT)
{
}

void
CD3D9GraphicsDevice::CRenderTextureAllocator::Initialize(
    __in CD3D9GraphicsDevice* pGraphicsDevice,
    UINT32 Levels,
    DWORD Usage,
    D3DFORMAT Format,
    D3DPOOL Pool
    )
{
    m_pGraphicsDevice = pGraphicsDevice;
    m_levels = Levels;
    m_usage = Usage;
    m_format = Format;
    m_pool = Pool;
}

__override __checkReturn HRESULT 
CD3D9GraphicsDevice::CRenderTextureAllocator::AllocateTexture(
    UINT32 Width,
    UINT32 Height,
    __deref_out ITexture** ppTexture
    )
{
    HRESULT hr = S_OK;
    CD3D9Texture* pNewTexture = NULL;

    IFC(m_pGraphicsDevice->AllocateTexture(Width, Height, m_levels, m_usage, m_format, m_pool, &pNewTexture));

    *ppTexture = pNewTexture;
    pNewTexture = NULL;

Cleanup:
    ReleaseObject(pNewTexture);

    return hr;
}

__override __checkReturn HRESULT 
CD3D9GraphicsDevice::CRenderTextureAllocator::AllocateTexture(
    UINT32 Width,
    UINT32 Height,
    __deref_out IBatchUpdateTexture** ppTexture
    )
{
    HRESULT hr = S_OK;
    CD3D9Texture* pNewTexture = NULL;

    IFC(m_pGraphicsDevice->AllocateTexture(Width, Height, m_levels, m_usage, m_format, m_pool, &pNewTexture));

    *ppTexture = pNewTexture;
    pNewTexture = NULL;

Cleanup:
    ReleaseObject(pNewTexture);

    return hr;
}

#endif