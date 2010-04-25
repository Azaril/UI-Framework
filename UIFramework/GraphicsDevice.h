#pragma once

#include "RefCounted.h"
#include "RenderTarget.h"
#include "TextProvider.h"

class CGraphicsDevice : public CRefCountedObject
{
	public:
		virtual HRESULT CreateHWNDRenderTarget( HWND Window, CRenderTarget** ppRenderTarget ) = 0;
        //virtual HRESULT CreateRenderTarget( const SizeF& Size, CRenderTarget** ppRenderTarget ) = 0;

        virtual HRESULT GetTextProvider( CTextProvider** ppTextProvider ) = 0;
};

HRESULT CreateGraphicsDevice( CGraphicsDevice** ppGraphicsDevice );