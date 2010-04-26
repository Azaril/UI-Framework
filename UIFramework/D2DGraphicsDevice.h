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

    protected:
        CD2DGraphicsDevice();
        virtual ~CD2DGraphicsDevice();

        HRESULT Initialize( );

        virtual HRESULT CreateTextProvider( CTextProvider** ppTextProvider );

        HMODULE m_D2DModule;
        ID2D1Factory* m_Factory;
        CTextProvider* m_TextProvider;
};
