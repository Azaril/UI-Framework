#pragma once

#include "Factory.h"
#include "GraphicsDevice.h"

#include <d2d1.h>
#include <d2d1helper.h>

class CD2DGraphicsDevice : public CGraphicsDevice
{
    public:
        DECLARE_FACTORY( CD2DGraphicsDevice );

		virtual HRESULT CreateHWNDRenderTarget( HWND Window, CRenderTarget** ppRenderTarget );
        //virtual HRESULT CreateRenderTarget( const SizeF& Size, CRenderTarget** ppRenderTarget );

        virtual HRESULT GetTextProvider( CTextProvider** ppTextProvider );

        virtual HRESULT GetImagingProvider( CImagingProvider** ppImagingProvider );

        virtual HRESULT CreateRectangleGeometry( const RectF& Rectangle, CRectangleGeometry** ppRectangleGeometry );

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
