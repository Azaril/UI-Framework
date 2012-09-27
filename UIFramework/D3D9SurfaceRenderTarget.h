#pragma once

#if defined(FRAMEWORK_D3D9)

#include "D3D9RenderTarget.h"

class CD3D9SurfaceRenderTarget : public CD3D9RenderTarget
{
    public:
        DECLARE_FACTORY3( CD3D9SurfaceRenderTarget, IDirect3DDevice9*, IDirect3DSurface9*, CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >* );

        __override virtual SizeF GetSize(
            );

        __override virtual __checkReturn HRESULT BeginRendering(
            );

        __override virtual __checkReturn HRESULT EndRendering(
            );

    protected:
        CD3D9SurfaceRenderTarget(
            );

        virtual ~CD3D9SurfaceRenderTarget(
            );

        __checkReturn HRESULT Initialize(
            __in IDirect3DDevice9* pDevice,
            __in IDirect3DSurface9* pSurface,
            __in CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >* pTextureAtlasPool
            );

        IDirect3DSurface9* m_pSurface;
        UINT32 m_Width;
        UINT32 m_Height;
};

#endif