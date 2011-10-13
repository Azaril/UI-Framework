#pragma once

#include "D3D10RenderTarget.h"

class UIFRAMEWORK_API CD3D10HWNDRenderTarget : public CD3D10RenderTarget
{
    public:
        DECLARE_FACTORY2( CD3D10HWNDRenderTarget, ID3D10Device*, IDXGISwapChain* );

        __override virtual SizeF GetSize(
            );

        __override virtual __checkReturn HRESULT BeginRendering(
            );

        __override virtual __checkReturn HRESULT EndRendering(
            );

    protected:
        CD3D10HWNDRenderTarget(
            );

        virtual ~CD3D10HWNDRenderTarget(
            );

        __checkReturn HRESULT Initialize(
            __in ID3D10Device* pDevice,
            __in IDXGISwapChain* pSwapChain
            );

        UINT32 m_Width;
        UINT32 m_Height;
        IDXGISwapChain* m_pSwapChain;
        ID3D10Texture2D* m_pBackBuffer;
        ID3D10RenderTargetView* m_pBackBufferView;
};