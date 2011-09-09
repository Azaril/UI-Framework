#pragma once

#include "Factory.h"
#include "GraphicsDevice.h"

#include <d2d1.h>
#include <d2d1helper.h>

class CD2DHWNDRenderTarget;

class UIFRAMEWORK_API CD2DGraphicsDevice : public CGraphicsDevice
{
    public:
        DECLARE_FACTORY( CD2DGraphicsDevice );

		__checkReturn HRESULT CreateHWNDRenderTarget( 
			HWND Window, 
			__deref_out CD2DHWNDRenderTarget** ppRenderTarget 
			);

        __checkReturn HRESULT CreateDXGISurfaceRenderTarget( 
			__in IDXGISurface* pSurface, 
			__deref_out CRenderTarget** ppRenderTarget 
			);

        __override virtual __checkReturn HRESULT GetTextProvider(
            __deref_out CTextProvider** ppTextProvider 
            );

        __override virtual __checkReturn HRESULT GetImagingProvider( 
            __deref_out CImagingProvider** ppImagingProvider 
            );

        __override virtual __checkReturn HRESULT CreateRectangleGeometry( 
            const RectF& Rectangle, 
            __deref_out CRectangleGeometry** ppRectangleGeometry
            );

        __override virtual __checkReturn HRESULT CreateRoundedRectangleGeometry( 
            const RectF& Rectangle, 
            FLOAT CornerRadius, 
            __deref_out CRoundedRectangleGeometry** ppRectangleGeometry 
            );

    protected:
        CD2DGraphicsDevice(
            );

        virtual ~CD2DGraphicsDevice(
            );

        __checkReturn HRESULT Initialize(
            );

        virtual __checkReturn HRESULT CreateTextProvider( 
            __deref_out CTextProvider** ppTextProvider
            );

        virtual __checkReturn HRESULT CreateImagingProvider( 
            __deref_out CImagingProvider** ppImagingProvider 
            );

        HMODULE m_D2DModule;
        ID2D1Factory* m_Factory;
        CTextProvider* m_TextProvider;
        CImagingProvider* m_ImagingProvider;
};
