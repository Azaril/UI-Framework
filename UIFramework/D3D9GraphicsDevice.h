#pragma once

#if defined(FRAMEWORK_D3D9)

#include "Types.h"
#include <d3d9.h>

#include "Factory.h"
#include "GraphicsDevice.h"
#include "D3D9RenderTarget.h"
#include "D3D9HWNDRenderTarget.h"
#include "D3D9SurfaceRenderTarget.h"
#include "StagingTextureCallback.h"

class CD3D9Texture;
class CStagingTextureWrapper;

class UIFRAMEWORK_API CD3D9GraphicsDevice : public CGraphicsDevice,
                                            public IBatchUpdateTextureAllocator,
                                            private IStagingTextureCallback
{
    public:
        DECLARE_FACTORY1( CD3D9GraphicsDevice, IDirect3DDevice9* );

#if !defined(_XBOX)
        DECLARE_FACTORY1( CD3D9GraphicsDevice, HWND );      

        __checkReturn HRESULT CreateHWNDRenderTarget(
            HWND Window,
            __deref_out CD3D9HWNDRenderTarget** ppRenderTarget
            );
#endif

        __checkReturn HRESULT CreateSurfaceRenderTarget(
            __in IDirect3DSurface9* pSurface,
            __deref_out CD3D9SurfaceRenderTarget** ppRenderTarget
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
        CD3D9GraphicsDevice(
            );

        virtual ~CD3D9GraphicsDevice(
            );

        __checkReturn HRESULT Initialize(
            IDirect3DDevice9* pDevice
            );

#if !defined(_XBOX)
        __checkReturn HRESULT Initialize(
            HWND focusWindow
            );
#endif

        __checkReturn HRESULT InitializeCommon(
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

        __override virtual __checkReturn HRESULT AllocateTexture(
            UINT32 Width,
            UINT32 Height,
            __deref_out IBatchUpdateTexture** ppTexture
            );

        virtual __checkReturn HRESULT AllocateTexture(
            UINT32 Width,
            UINT32 Height,
            __deref_out CStagingTextureWrapper** ppTexture
            );

        __checkReturn HRESULT AllocateTexture(
            UINT32 Width,
            UINT32 Height,
            UINT32 Levels,
            DWORD Usage,
            D3DFORMAT Format,
            D3DPOOL Pool,
            __deref_out CD3D9Texture** ppTexture
            );

        __override virtual __checkReturn HRESULT AddUpdate(
            __in ITexture* pSource,
            const RectU& sourceRect,
            __in ITexture* pDestination,
            const Point2U& destOffset
            );

        HWND m_FocusWindow;
        HMODULE m_D3D9Module;
        IDirect3D9* m_pD3D;
        IDirect3DDevice9* m_pDevice;

        CTextProvider* m_pTextProvider;
        CImagingProvider* m_pImagingProvider;
        CGeometryProvider* m_pGeometryProvider;

        typedef CTextureAtlasWithWhitePixel< 1 > RenderTextureAtlasType;
        typedef CTextureAtlas< ITextureAtlas, 0 > StagingTextureAtlasType;

        CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >* m_pTextureAtlasPool;
        CTextureAtlasPool< CTextureAtlas< ITextureAtlas, 0 > >* m_pStagingTextureAtlasPool;

#if defined(_XBOX)
        class UIFRAMEWORK_API CRenderTextureAllocator : public IBatchUpdateTextureAllocator
        {
            public:
                CRenderTextureAllocator(
                    );

                void Initialize(
                    PixelFormat::Value format
                    );

                __override virtual __checkReturn HRESULT AllocateTexture(
                    UINT32 Width,
                    UINT32 Height,
                    __deref_out ITexture** ppTexture
                    );

                __override virtual __checkReturn HRESULT AllocateTexture(
                    UINT32 Width,
                    UINT32 Height,
                    __deref_out IBatchUpdateTexture** ppTexture
                    );

            protected:
                PixelFormat::Value m_format;
        };
#else
        class UIFRAMEWORK_API CRenderTextureAllocator : public IBatchUpdateTextureAllocator
        {
            public:
                CRenderTextureAllocator(
                    );

                void Initialize(
                    __in CD3D9GraphicsDevice* pGraphicsDevice,
                    UINT32 Levels,
                    DWORD Usage,
                    D3DFORMAT Format,
                    D3DPOOL Pool
                    );

                __override virtual __checkReturn HRESULT AllocateTexture(
                    UINT32 Width,
                    UINT32 Height,
                    __deref_out ITexture** ppTexture
                    );

                __override virtual __checkReturn HRESULT AllocateTexture(
                    UINT32 Width,
                    UINT32 Height,
                    __deref_out IBatchUpdateTexture** ppTexture
                    );

            protected:
                CD3D9GraphicsDevice* m_pGraphicsDevice;
                UINT32 m_levels;
                DWORD m_usage;
                D3DFORMAT m_format;
                D3DPOOL m_pool;
        };
#endif

        CRenderTextureAllocator m_StagingTextureAllocator;

#if defined(_XBOX) && defined(FRAMEWORK_DEBUG)
		UINT32 m_allocatedTextureCount;
#endif
};

#endif