#pragma once

#include <OpenGLES/ES2/gl.h>

#include "Factory.h"
#include "GraphicsDevice.h"
#include "OpenGLES20Context.h"
#include "OpenGLES20RenderBufferStorageAllocator.h"
#include "OpenGLES20Texture.h"
#include "TextureAtlasPool.h"
#include "TextureAtlasWithWhitePixel.h"

class COpenGLES20RenderTarget;

class COpenGLES20GraphicsDevice : public CGraphicsDevice,
                                  public IBatchUpdateTextureAllocator
{
	public:
		DECLARE_FACTORY1( COpenGLES20GraphicsDevice, COpenGLES20Context* );

		__checkReturn HRESULT CreateRenderTarget(
			__in COpenGLES20RenderBufferStorageAllocator* pAllocator,
			__deref_out COpenGLES20RenderTarget** ppRenderTarget
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
		COpenGLES20GraphicsDevice(
			);

		virtual ~COpenGLES20GraphicsDevice(
			);
    
        __checkReturn HRESULT Initialize(
            __in_opt COpenGLES20Context* pContext
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
    
        __override virtual __checkReturn HRESULT AllocateTexture(
            UINT32 Width,
            UINT32 Height,
            __deref_out ITexture** ppTexture
            );
    
        __override virtual __checkReturn HRESULT AllocateTexture(
            UINT32 Width,
            UINT32 Height,
            __deref_out IBatchUpdateTexture** ppTexture
            );    

        __checkReturn HRESULT CreateTexture(
            UINT32 Width,
            UINT32 Height,
            __deref_out COpenGLES20Texture** ppTexture
            );    

        COpenGLES20Context* m_pContext;
        CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >* m_pTextureAtlasPool;
    
        CTextProvider* m_pTextProvider;
        CImagingProvider* m_pImagingProvider;
        CGeometryProvider* m_pGeometryProvider;
};