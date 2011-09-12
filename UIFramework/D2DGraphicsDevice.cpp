#include "D2DGraphicsDevice.h"
#include "D2DHWNDRenderTarget.h"
#include "D2DSurfaceRenderTarget.h"
#include "D2DBitmapRenderTarget.h"
#include "DirectWriteTextProvider.h"
#include "WICImagingProvider.h"
#include "D2DRectangleGeometry.h"
#include "D2DRoundedRectangleGeometry.h"
#include "D2DGeometryProvider.h"

typedef HRESULT (WINAPI *D2D1CreateFactoryFunc)( 
	__in D2D1_FACTORY_TYPE factoryType, 
	__in REFIID riid, 
	__in_opt CONST D2D1_FACTORY_OPTIONS *pFactoryOptions, 
	__out void **ppIFactory 
	);

CD2DGraphicsDevice::CD2DGraphicsDevice(
	) 
	: m_D2DModule(NULL)
	, m_Factory(NULL)
	, m_TextProvider(NULL)
	, m_ImagingProvider(NULL)
    , m_GeometryProvider(NULL)
{
}

CD2DGraphicsDevice::~CD2DGraphicsDevice(
	)
{
    ReleaseObject(m_Factory);
    ReleaseObject(m_TextProvider);
    ReleaseObject(m_ImagingProvider);
    ReleaseObject(m_GeometryProvider);

    if(m_D2DModule != NULL)
    {
        FreeLibrary(m_D2DModule);
    }
}

__checkReturn HRESULT 
CD2DGraphicsDevice::Initialize(
	)
{
    HRESULT hr = S_OK;
    D2D1CreateFactoryFunc CreateFactory = NULL;

    m_D2DModule = LoadLibrary(L"d2d1.dll");
    IFCEXPECT(m_D2DModule != NULL);

    CreateFactory = (D2D1CreateFactoryFunc)GetProcAddress(m_D2DModule, "D2D1CreateFactory");
    IFCPTR(CreateFactory);

    IFC(CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, __uuidof(ID2D1Factory), NULL, (void**)&m_Factory));

    IFC(CreateTextProvider(&m_TextProvider));

    IFC(CreateImagingProvider(&m_ImagingProvider));

    IFC(CreateGeometryProvider(&m_GeometryProvider));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CD2DGraphicsDevice::CreateHWNDRenderTarget(
	HWND Window, 
	__deref_out CD2DHWNDRenderTarget** ppRenderTarget
	)
{
    HRESULT hr = S_OK;
    RECT ClientRect = { 0 };
    ID2D1HwndRenderTarget* pD2DRenderTarget = NULL;
    CD2DHWNDRenderTarget* pHWNDRenderTarget = NULL;
    D2D1_SIZE_U Size;

    IFCEXPECT(Window != NULL);
    IFCPTR(ppRenderTarget);

    IFCEXPECT(GetClientRect(Window, &ClientRect));

    Size = D2D1::SizeU(ClientRect.right - ClientRect.left, ClientRect.bottom - ClientRect.top);

    IFC(m_Factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(Window, Size), &pD2DRenderTarget));

    IFC(CD2DHWNDRenderTarget::Create(pD2DRenderTarget, &pHWNDRenderTarget));

    *ppRenderTarget = pHWNDRenderTarget;
    pHWNDRenderTarget = NULL;

Cleanup:
    ReleaseObject(pD2DRenderTarget);
    ReleaseObject(pHWNDRenderTarget);

    return hr;
}

__checkReturn HRESULT 
CD2DGraphicsDevice::CreateDXGISurfaceRenderTarget(
	__in IDXGISurface* pSurface, 
	__deref_out CRenderTarget** ppRenderTarget
	)
{
    HRESULT hr = S_OK;
    ID2D1RenderTarget* pD2DRenderTarget = NULL;
    CD2DSurfaceRenderTarget* pSurfaceRenderTarget = NULL;

    IFCPTR(pSurface);
    IFCPTR(ppRenderTarget);

    IFC(m_Factory->CreateDxgiSurfaceRenderTarget(pSurface, D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)), &pD2DRenderTarget));

    IFC(CD2DSurfaceRenderTarget::Create(pSurface, pD2DRenderTarget, &pSurfaceRenderTarget));

    *ppRenderTarget = pSurfaceRenderTarget;
    pSurfaceRenderTarget = NULL;

Cleanup:
    ReleaseObject(pD2DRenderTarget);
    ReleaseObject(pSurfaceRenderTarget);

    return hr;
}

__override __checkReturn HRESULT 
CD2DGraphicsDevice::GetTextProvider(
	__deref_out CTextProvider** ppTextProvider
	)
{
    HRESULT hr = S_OK;

    IFCPTR(ppTextProvider);

    *ppTextProvider = m_TextProvider;
    AddRefObject(m_TextProvider);

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CD2DGraphicsDevice::GetImagingProvider(
	CImagingProvider** ppImagingProvider
	)
{
    HRESULT hr = S_OK;

    IFCPTR(ppImagingProvider);

    *ppImagingProvider = m_ImagingProvider;
    AddRefObject(m_ImagingProvider);

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CD2DGraphicsDevice::GetGeometryProvider(
	CGeometryProvider** ppGeometryProvider
	)
{
    HRESULT hr = S_OK;

    IFCPTR(ppGeometryProvider);

    *ppGeometryProvider = m_GeometryProvider;
    AddRefObject(m_GeometryProvider);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CD2DGraphicsDevice::CreateTextProvider(
	__deref_out CTextProvider** ppTextProvider
	)
{
    HRESULT hr = S_OK;
    CDirectWriteTextProvider* pDirectWriteTextProvider = NULL;

    IFCPTR(ppTextProvider);

    if(SUCCEEDED(CDirectWriteTextProvider::Create(&pDirectWriteTextProvider)))
    {
        *ppTextProvider = pDirectWriteTextProvider;
        pDirectWriteTextProvider = NULL;
        goto Cleanup;
    }

    IFC(E_FAIL);

Cleanup:
    ReleaseObject(pDirectWriteTextProvider);

    return hr;
}

__checkReturn HRESULT 
CD2DGraphicsDevice::CreateImagingProvider(
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
CD2DGraphicsDevice::CreateGeometryProvider(
	__deref_out CGeometryProvider** ppGeometryProvider
	)
{
    HRESULT hr = S_OK;
    CD2DGeometryProvider* pD2DGeometryProvider = NULL;

    IFCPTR(ppGeometryProvider);

    if(SUCCEEDED(CD2DGeometryProvider::Create(m_Factory, &pD2DGeometryProvider)))
    {
        *ppGeometryProvider = pD2DGeometryProvider;
        pD2DGeometryProvider = NULL;
        goto Cleanup;
    }

    IFC(E_FAIL);

Cleanup:
    ReleaseObject(pD2DGeometryProvider);

    return hr;
}