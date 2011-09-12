#pragma once

#import <OpenGLES/ES2/gl.h>

#include "Factory.h"
#include "GraphicsDevice.h"
#include "OpenGLES20Context.h"
#include "OpenGLES20RenderBufferStorageAllocator.h"

class COpenGLES20RenderTarget;

class COpenGLES20GraphicsDevice : public CGraphicsDevice
{
	public:
		DECLARE_FACTORY( COpenGLES20GraphicsDevice );

		__checkReturn HRESULT CreateRenderTarget(
			__in COpenGLES20RenderBufferStorageAllocator* pAllocator,
			__in_opt COpenGLES20Context* pContext,
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
            );

        virtual __checkReturn HRESULT CreateTextProvider( 
            __deref_out CTextProvider** ppTextProvider
            );

        virtual __checkReturn HRESULT CreateImagingProvider( 
            __deref_out CImagingProvider** ppImagingProvider 
            );            

        CTextProvider* m_pTextProvider;
        CImagingProvider* m_pImagingProvider;
        CGeometryProvider* m_pGeometryProvider;
};