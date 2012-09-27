#pragma once

#if defined(FRAMEWORK_D3D9)

#include "D3D9RenderTarget.h"

#if !defined(_XBOX)

class UIFRAMEWORK_API CD3D9HWNDRenderTarget : public CD3D9RenderTarget
{
    public:
        DECLARE_FACTORY3( CD3D9HWNDRenderTarget, IDirect3DDevice9*, IDirect3DSwapChain9*, CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >* );

        __override virtual SizeF GetSize(
            );

        __override virtual __checkReturn HRESULT BeginRendering(
            );

        __override virtual __checkReturn HRESULT EndRendering(
            );

    protected:
        CD3D9HWNDRenderTarget(
            );

        virtual ~CD3D9HWNDRenderTarget(
            );

        __checkReturn HRESULT Initialize(
            __in IDirect3DDevice9* pDevice,
            __in IDirect3DSwapChain9* pSwapChain,
            __in CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >* pTextureAtlasPool
            );

        UINT32 m_Width;
        UINT32 m_Height;
        IDirect3DSwapChain9* m_pSwapChain;
};

#endif

#endif