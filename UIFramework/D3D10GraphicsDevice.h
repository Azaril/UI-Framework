#pragma once

#include <d3d10.h>

#include "Factory.h"
#include "GraphicsDevice.h"
#include "D3D10RenderTarget.h"
#include "D3D10HWNDRenderTarget.h"

typedef HRESULT (WINAPI *D3D10CreateDeviceFunc)(
    __in IDXGIAdapter* pAdapter,
    __in D3D10_DRIVER_TYPE DriverType,
    __in HMODULE Software,
    __in UINT Flags,
    __in UINT SDKVersion,
    __deref_out ID3D10Device** ppDevice
    );

typedef HRESULT (WINAPI *D3D10CreateDeviceAndSwapChainFunc)(
    __in IDXGIAdapter *pAdapter,
    __in D3D10_DRIVER_TYPE DriverType,
    __in HMODULE Software,
    __in UINT Flags,
    __in UINT SDKVersion,
    __in DXGI_SWAP_CHAIN_DESC *pSwapChainDesc,
    __out IDXGISwapChain **ppSwapChain,
    __out ID3D10Device **ppDevice
    );

class UIFRAMEWORK_API CD3D10GraphicsDevice : public CGraphicsDevice
{
    public:
        DECLARE_FACTORY( CD3D10GraphicsDevice );

        __checkReturn HRESULT CreateHWNDRenderTarget(
            HWND Window,
            __deref_out CD3D10HWNDRenderTarget** ppRenderTarget
            );

        __override virtual __checkReturn HRESULT GetTextProvider(
            __deref_out CTextProvider** ppTextProvider 
            );

        __override virtual __checkReturn HRESULT GetImagingProvider( 
            __deref_out CImagingProvider** ppImagingProvider 
            );

        __override virtual __checkReturn HRESULT GetGeometryProvider(
            __deref_out CGeometryProvider** ppGeometryProvider
            );

    protected:
        CD3D10GraphicsDevice(
            );

        virtual ~CD3D10GraphicsDevice(
            );

        __checkReturn HRESULT Initialize(
            );

        virtual __checkReturn HRESULT CreateTextProvider( 
            __deref_out CTextProvider** ppTextProvider
            );

        virtual __checkReturn HRESULT CreateImagingProvider( 
            __deref_out CImagingProvider** ppImagingProvider 
            );            

        virtual __checkReturn HRESULT CreateGeometryProvider(
            __deref_out CGeometryProvider** ppGeometryProvider
            );

        HMODULE m_D3D10Module;
        D3D10CreateDeviceFunc m_pCreateDevice;
        D3D10CreateDeviceAndSwapChainFunc m_pCreateDeviceAndSwapChain;

        CTextProvider* m_pTextProvider;
        CImagingProvider* m_pImagingProvider;
        CGeometryProvider* m_pGeometryProvider;
};