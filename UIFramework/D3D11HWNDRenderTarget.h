#pragma once

#include "D3D11RenderTarget.h"

class UIFRAMEWORK_API CD3D11HWNDRenderTarget : public CD3D11RenderTarget
{
    public:
        DECLARE_FACTORY3( CD3D11HWNDRenderTarget, ID3D11Device*, IDXGISwapChain*, CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >* );

        __override virtual SizeF GetSize(
            );

        __override virtual __checkReturn HRESULT BeginRendering(
            );

        __override virtual __checkReturn HRESULT EndRendering(
            );

    protected:
        CD3D11HWNDRenderTarget(
            );

        virtual ~CD3D11HWNDRenderTarget(
            );

        __checkReturn HRESULT Initialize(
            __in ID3D11Device* pDevice,
            __in IDXGISwapChain* pSwapChain,
            __in CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >* pTextureAtlasPool
            );

        UINT32 m_Width;
        UINT32 m_Height;
        IDXGISwapChain* m_pSwapChain;
        ID3D11Texture2D* m_pBackBuffer;
        ID3D11RenderTargetView* m_pBackBufferView;
};