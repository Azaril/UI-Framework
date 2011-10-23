#pragma once

#include <OpenGLES/ES2/gl.h>

#include "RenderTarget.h"
#include "OpenGLES20Context.h"
#include "OpenGLES20VertexBuffer.h"
#include "GeometryTesselationSink.h"
#include "OpenGLES20Texture.h"
#include "GraphicsBrushBase.h"
#include "TextureAtlasWithWhitePixel.h"
#include "TextureAllocator.h"
#include "TextureAtlasPool.h"
#include "RenderTargetBase.h"
#include "VertexBuffer.h"

class UIFRAMEWORK_API COpenGLES20RenderTarget : public CRenderTargetBase
{
    public:
    	DECLARE_FACTORY4( COpenGLES20RenderTarget, GLuint, GLuint, COpenGLES20Context*, CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >* );
    
        GLuint GetRenderBuffer(
            );

        __override virtual SizeF GetSize(
			);

        __override virtual __checkReturn HRESULT BeginRendering(
			);

        __override virtual __checkReturn HRESULT EndRendering(
			);

        __override virtual __checkReturn HRESULT Clear( 
			const ColorF& Color 
			);
    
    protected:
        COpenGLES20RenderTarget(
			);

        virtual ~COpenGLES20RenderTarget(
			);

        __checkReturn HRESULT Initialize(
			__in GLuint RenderBuffer,
			__in GLuint FrameBuffer,
			__in_opt COpenGLES20Context* pContext,
            __in CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >* pTextureAtlasPool
			);

		__override __checkReturn HRESULT ApplyContext(
			);

        __checkReturn HRESULT CreateShader(
			const GLenum ShaderType,
			__in_z const CHAR* pShaderSource,
			__out GLuint* pShader
			);
    
        __checkReturn HRESULT LinkProgram(
            GLuint Program
            );
    
        __override __checkReturn HRESULT OnTesselatedGeometryBatch(
            __in IVertexBuffer* pVertexBuffer
            );
    
        __override __checkReturn HRESULT Flush(
            );
    
        __override virtual __checkReturn HRESULT BindTexture(
            __in ITexture* pTexture
            );
    
        __override virtual __checkReturn HRESULT BindMask(
            __in ITexture* pTexture
            );

		GLuint m_RenderBuffer;
		GLuint m_FrameBuffer;
		GLint m_Width;
		GLint m_Height;
		COpenGLES20Context* m_pContext;
        COpenGLES20VertexBuffer* m_pVertexBuffers[2];
        GLuint m_ShaderProgram;
        GLint m_PositionAttribute;
        GLint m_ColorAttribute;
        GLint m_TextureCoordsAttribute;
        GLint m_MaskCoordsAttribute;
        GLint m_TransformUniform;
        GLint m_BrushTextureUniform;
        GLint m_MaskTextureUniform;
};
