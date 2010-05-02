#pragma once

#include "RefCounted.h"
#include "RenderTarget.h"
#include "TextProvider.h"
#include "ImagingProvider.h"

class CRenderTarget;

class CGraphicsDevice : public CRefCountedObject
{
	public:
		virtual HRESULT CreateHWNDRenderTarget( HWND Window, CRenderTarget** ppRenderTarget ) = 0;
        //virtual HRESULT CreateRenderTarget( const SizeF& Size, CRenderTarget** ppRenderTarget ) = 0;

        virtual HRESULT GetTextProvider( CTextProvider** ppTextProvider ) = 0;

        virtual HRESULT GetImagingProvider( CImagingProvider** ppImagingProvider ) = 0;

        virtual HRESULT CreateRectangleGeometry( const RectF& Rectangle, CRectangleGeometry** ppRectangleGeometry ) = 0;
};

HRESULT CreateGraphicsDevice( CGraphicsDevice** ppGraphicsDevice );