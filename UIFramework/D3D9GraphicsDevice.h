#pragma once

#include <d3d9.h>

#include "Factory.h"
#include "GraphicsDevice.h"
#include "D3D9RenderTarget.h"
#include "D3D9HWNDRenderTarget.h"

class UIFRAMEWORK_API CD3D9GraphicsDevice : public CGraphicsDevice
{
    public:
        DECLARE_FACTORY( CD3D9GraphicsDevice );

        __checkReturn HRESULT CreateHWNDRenderTarget(
            HWND Window,
            __deref_out CD3D9HWNDRenderTarget** ppRenderTarget
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

        HMODULE m_D3D9Module;
        IDirect3D9* m_pD3D;

        CTextProvider* m_pTextProvider;
        CImagingProvider* m_pImagingProvider;
        CGeometryProvider* m_pGeometryProvider;
};