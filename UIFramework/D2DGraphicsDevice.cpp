#include "D2DGraphicsDevice.h"
#include "D2DHWNDRenderTarget.h"
#include "D2DBitmapRenderTarget.h"
#include "DirectWriteTextProvider.h"

typedef HRESULT (WINAPI *D2D1CreateFactoryFunc)( __in D2D1_FACTORY_TYPE factoryType, __in REFIID riid, __in_opt CONST D2D1_FACTORY_OPTIONS *pFactoryOptions, __out void **ppIFactory );

CD2DGraphicsDevice::CD2DGraphicsDevice() : m_D2DModule(NULL),
                                           m_Factory(NULL),
                                           m_TextProvider(NULL)
{
}

CD2DGraphicsDevice::~CD2DGraphicsDevice()
{
    ReleaseObject(m_Factory);
    ReleaseObject(m_TextProvider);

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

    IFC(CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), NULL, (void**)&m_Factory));

    IFC(CreateTextProvider(&m_TextProvider));

Cleanup:
    return hr;
}

HRESULT CD2DGraphicsDevice::CreateHWNDRenderTarget(HWND Window, CRenderTarget** ppRenderTarget)
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

HRESULT CD2DGraphicsDevice::CreateTextProvider(CTextProvider** ppTextProvider)
{
    HRESULT hr = S_OK;
    CDirectWriteTextProvider* pDirectWriteTextProvider = NULL;

    IFCPTR(ppTextProvider);

    if(SUCCEEDED(CDirectWriteTextProvider::Create(this, &pDirectWriteTextProvider)))
    {
        *ppTextProvider = pDirectWriteTextProvider;
        pDirectWriteTextProvider = NULL;
        goto Cleanup;
    }

Cleanup:
    ReleaseObject(pDirectWriteTextProvider);

    return hr;
}