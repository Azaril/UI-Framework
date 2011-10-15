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

typedef HRESULT (WINAPI* CreateDXGIFactoryFunc)(
    __in REFIID riid,
    __deref_out void** ppFactory
    );

/*
typedef HRESULT (WINAPI *CreateSwapChainFunc)(
    __in IUnknown* pDevice,
    __in DXGI_SWAP_CHAIN_DESC* pDesc,
    __deref_out IDXGISwapChain** ppSwapChain
    );
    */

class UIFRAMEWORK_API CD3D10GraphicsDevice : public CGraphicsDevice,
                                             public ITextureAllocator,
                                             private IStagingTextureCallback
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

        __override virtual __checkReturn HRESULT AllocateTexture(
            UINT32 Width,
            UINT32 Height,
            __deref_out ITexture** ppTexture
            );

        __checkReturn HRESULT AllocateTexture(
            const D3D10_TEXTURE2D_DESC& textureDescription,
            __deref_out ITexture** ppTexture
            );

        __override virtual __checkReturn HRESULT AddUpdate(
            __in ITexture* pSource,
            const RectU& sourceRect,
            __in ITexture* pDestination,
            const Point2U& destOffset
            );

        HMODULE m_D3D10Module;
        D3D10CreateDeviceFunc m_pCreateDevice;

        HMODULE m_DXGIModule;
        CreateDXGIFactoryFunc m_pCreateDXGIFactory;

        ID3D10Device* m_pDevice;
        IDXGIFactory* m_pDXGIFactory;

        CTextProvider* m_pTextProvider;
        CImagingProvider* m_pImagingProvider;
        CGeometryProvider* m_pGeometryProvider;

        typedef CTextureAtlasWithWhitePixel< 1 > RenderTextureAtlasType;
        typedef CTextureAtlas< ITextureAtlas, 0 > StagingTextureAtlasType;

        CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >* m_pTextureAtlasPool;
        CTextureAtlasPool< CTextureAtlas< ITextureAtlas, 0 > >* m_pStagingTextureAtlasPool;

        class CRenderTextureAllocator : public ITextureAllocator
        {
        public:
            CRenderTextureAllocator(
                );

            void Initialize(
                __in CD3D10GraphicsDevice* pGraphicsDevice,
                const D3D10_TEXTURE2D_DESC& baseDescription
                );

            __override virtual __checkReturn HRESULT AllocateTexture(
                UINT32 Width,
                UINT32 Height,
                __deref_out ITexture** ppTexture
                );

        protected:
            CD3D10GraphicsDevice* m_pGraphicsDevice;
            D3D10_TEXTURE2D_DESC m_TextureDescription;
        };

        CRenderTextureAllocator m_StagingTextureAllocator;
};