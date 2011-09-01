#pragma once

#include "RefCounted.h"
#include "RenderTarget.h"
#include "TextProvider.h"
#include "ImagingProvider.h"

class CRenderTarget;

class UIFRAMEWORK_API CGraphicsDevice : public CRefCountedObject
{
	public:
        virtual __checkReturn HRESULT GetTextProvider( 
			__deref_out CTextProvider** ppTextProvider 
			) = 0;

        virtual __checkReturn HRESULT GetImagingProvider( 
			__deref_out CImagingProvider** ppImagingProvider 
			) = 0;

        virtual __checkReturn HRESULT CreateRectangleGeometry( 
			const RectF& Rectangle, 
			__deref_out CRectangleGeometry** ppRectangleGeometry 
			) = 0;

        virtual __checkReturn HRESULT CreateRoundedRectangleGeometry( 
			const RectF& Rectangle, 
			FLOAT CornerRadius, 
			__deref_out CRoundedRectangleGeometry** ppRectangleGeometry 
			) = 0;
};

//HRESULT CreateGraphicsDevice( CGraphicsDevice** ppGraphicsDevice );