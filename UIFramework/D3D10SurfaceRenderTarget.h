#pragma once

#include "D3D10RenderTarget.h"

class CD3D10SurfaceRenderTarget : public CD3D10RenderTarget
{
    public:
        DECLARE_FACTORY3( CD3D10SurfaceRenderTarget, ID3D10Device*, ID3D10Texture2D*, CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >* );

        __override virtual SizeF GetSize(
            );

        __override virtual __checkReturn HRESULT BeginRendering(
            );

        __override virtual __checkReturn HRESULT EndRendering(
            );

    protected:
        CD3D10SurfaceRenderTarget(
            );

        virtual ~CD3D10SurfaceRenderTarget(
            );

        __checkReturn HRESULT Initialize(
            __in ID3D10Device* pDevice,
            __in ID3D10Texture2D* pTexture,
            __in CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >* pTextureAtlasPool
            );

        UINT32 m_Width;
        UINT32 m_Height;
        ID3D10Texture2D* m_pTexture;
        ID3D10RenderTargetView* m_pTextureView;
};

