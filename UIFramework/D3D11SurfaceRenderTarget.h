#pragma once

#if defined(FRAMEWORK_D3D11)

#include "D3D11RenderTarget.h"

class CD3D11SurfaceRenderTarget : public CD3D11RenderTarget
{
    public:
        DECLARE_FACTORY3( CD3D11SurfaceRenderTarget, ID3D11Device*, ID3D11Texture2D*, CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >* );

        __override virtual SizeF GetSize(
            );

        __override virtual __checkReturn HRESULT BeginRendering(
            );

        __override virtual __checkReturn HRESULT EndRendering(
            );

    protected:
        CD3D11SurfaceRenderTarget(
            );

        virtual ~CD3D11SurfaceRenderTarget(
            );

        __checkReturn HRESULT Initialize(
            __in ID3D11Device* pDevice,
            __in ID3D11Texture2D* pTexture,
            __in CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >* pTextureAtlasPool
            );

        UINT32 m_Width;
        UINT32 m_Height;
        ID3D11Texture2D* m_pTexture;
        ID3D11RenderTargetView* m_pTextureView;
};

#endif