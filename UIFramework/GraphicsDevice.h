#pragma once

#include "RefCounted.h"
#include "RenderTarget.h"
#include "TextProvider.h"
#include "ImagingProvider.h"
#include "GeometryProvider.h"

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

        virtual __checkReturn HRESULT GetGeometryProvider(
            __deref_out CGeometryProvider** ppGeometryProvider
            ) = 0;
};

//HRESULT CreateGraphicsDevice( CGraphicsDevice** ppGraphicsDevice );