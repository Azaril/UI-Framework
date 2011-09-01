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

        HRESULT CreateDXGISurfaceRenderTarget( 
			__in IDXGISurface* pSurface, 
			__deref_out CRenderTarget** ppRenderTarget 
			);

        __override virtual HRESULT GetTextProvider( CTextProvider** ppTextProvider );

        __override virtual HRESULT GetImagingProvider( CImagingProvider** ppImagingProvider );

        __override virtual HRESULT CreateRectangleGeometry( const RectF& Rectangle, CRectangleGeometry** ppRectangleGeometry );
        __override virtual HRESULT CreateRoundedRectangleGeometry( const RectF& Rectangle, FLOAT CornerRadius, CRoundedRectangleGeometry** ppRectangleGeometry );

    protected:
        CD2DGraphicsDevice();
        virtual ~CD2DGraphicsDevice();

        HRESULT Initialize( );

        virtual HRESULT CreateTextProvider( CTextProvider** ppTextProvider );
        virtual HRESULT CreateImagingProvider( CImagingProvider** ppImagingProvider );

        HMODULE m_D2DModule;
        ID2D1Factory* m_Factory;
        CTextProvider* m_TextProvider;
        CImagingProvider* m_ImagingProvider;
};
