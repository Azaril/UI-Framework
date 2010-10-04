#include "D2DGraphicsDevice.h"
#include "D2DHWNDRenderTarget.h"
#include "D2DSurfaceRenderTarget.h"
#include "D2DBitmapRenderTarget.h"
#include "DirectWriteTextProvider.h"
#include "WICImagingProvider.h"
#include "D2DRectangleGeometry.h"
#include "D2DRoundedRectangleGeometry.h"

typedef HRESULT (WINAPI *D2D1CreateFactoryFunc)( __in D2D1_FACTORY_TYPE factoryType, __in REFIID riid, __in_opt CONST D2D1_FACTORY_OPTIONS *pFactoryOptions, __out void **ppIFactory );

CD2DGraphicsDevice::CD2DGraphicsDevice() : m_D2DModule(NULL),
                                           m_Factory(NULL),
                                           m_TextProvider(NULL),
                                           m_ImagingProvider(NULL)
{
}

CD2DGraphicsDevice::~CD2DGraphicsDevice()
{
    ReleaseObject(m_Factory);
    ReleaseObject(m_TextProvider);
    ReleaseObject(m_ImagingProvider);

    if(m_D2DModule != NULL)
    {
        FreeLibrary(m_D2DModule);
    }
}

HRESULT CD2DGraphicsDevice::Initialize()
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

Cleanup:
    return hr;
}

HRESULT CD2DGraphicsDevice::CreateHWNDRenderTarget(HWND Window, CD2DHWNDRenderTarget** ppRenderTarget)
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

HRESULT CD2DGraphicsDevice::CreateDXGISurfaceRenderTarget(IDXGISurface* pSurface, CRenderTarget** ppRenderTarget)
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

//HRESULT CD2DGraphicsDevice::CreateRenderTarget(const SizeF& Size, CRenderTarget** ppRenderTarget)
//{
//    HRESULT hr = S_OK;
//
//    IFCPTR(ppRenderTarget);
//
//    IFC(m_Factory->CreateC
//
//Cleanup:
//    return hr;
//}

HRESULT CD2DGraphicsDevice::GetTextProvider(CTextProvider** ppTextProvider)
{
    HRESULT hr = S_OK;

    IFCPTR(ppTextProvider);

    *ppTextProvider = m_TextProvider;
    AddRefObject(m_TextProvider);

Cleanup:
    return hr;
}

HRESULT CD2DGraphicsDevice::GetImagingProvider(CImagingProvider** ppImagingProvider)
{
    HRESULT hr = S_OK;

    IFCPTR(ppImagingProvider);

    *ppImagingProvider = m_ImagingProvider;
    AddRefObject(m_ImagingProvider);

Cleanup:
    return hr;
}

HRESULT CD2DGraphicsDevice::CreateTextProvider(CTextProvider** ppTextProvider)
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

HRESULT CD2DGraphicsDevice::CreateImagingProvider(CImagingProvider** ppImagingProvider)
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

HRESULT CD2DGraphicsDevice::CreateRectangleGeometry(const RectF& Rectangle, CRectangleGeometry** ppRectangleGeometry)
{
    HRESULT hr = S_OK;
    ID2D1RectangleGeometry* pD2DRectangleGeometry = NULL;
    CD2DRectangleGeometry* pGeometry = NULL;

    IFCPTR(ppRectangleGeometry);

    IFC(m_Factory->CreateRectangleGeometry(Rectangle, &pD2DRectangleGeometry));

    IFC(CD2DRectangleGeometry::Create(pD2DRectangleGeometry, &pGeometry));

    *ppRectangleGeometry = pGeometry;
    pGeometry = NULL;

Cleanup:
    ReleaseObject(pD2DRectangleGeometry);
    ReleaseObject(pGeometry);

    return hr;
}

HRESULT CD2DGraphicsDevice::CreateRoundedRectangleGeometry(const RectF& Rectangle, FLOAT CornerRadius, CRoundedRectangleGeometry** ppRoundedRectangleGeometry)
{
    HRESULT hr = S_OK;
    ID2D1RoundedRectangleGeometry* pD2DRoundedRectangleGeometry = NULL;
    CD2DRoundedRectangleGeometry* pGeometry = NULL;
    D2D1_ROUNDED_RECT RoundedRect = { Rectangle, CornerRadius, CornerRadius };

    IFCPTR(ppRoundedRectangleGeometry);

    IFC(m_Factory->CreateRoundedRectangleGeometry(RoundedRect, &pD2DRoundedRectangleGeometry));

    IFC(CD2DRoundedRectangleGeometry::Create(pD2DRoundedRectangleGeometry, &pGeometry));

    *ppRoundedRectangleGeometry = pGeometry;
    pGeometry = NULL;

Cleanup:
    ReleaseObject(pD2DRoundedRectangleGeometry);
    ReleaseObject(pGeometry);

    return hr;
}