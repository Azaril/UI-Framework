#pragma once

#include <OpenGLES/ES2/gl.h>

#include "RenderTarget.h"
#include "OpenGLES20Context.h"
#include "OpenGLES20VertexBuffer.h"
#include "OpenGLES20TesselationSink.h"
#include "OpenGLES20Texture.h"
#include "OpenGLES20Brush.h"

class UIFRAMEWORK_API COpenGLES20RenderTarget : public CRenderTarget,
                                                private ITesselationBatchCallback
{
    public:
    	DECLARE_FACTORY3( COpenGLES20RenderTarget, GLuint, GLuint, COpenGLES20Context* );
    
        GLuint GetRenderBuffer(
            );

        __override virtual SizeF GetSize(
			);

        __override virtual __checkReturn HRESULT BeginRendering(
			);

        __override virtual __checkReturn HRESULT EndRendering(
			);

        __override virtual __checkReturn HRESULT SetTransform( 
			const Matrix3X2F& Transform 
			);

        __override virtual __checkReturn HRESULT Clear( 
			const ColorF& Color 
			);

        __override virtual __checkReturn HRESULT CreateSolidBrush( 
			const ColorF& Color, 
			__deref_out CGraphicsBrush** ppBrush 
			);

        __override virtual __checkReturn HRESULT CreateLinearGradientBrush( 	
			const Point2F& StartPoint, 
			const Point2F& EndPoint, 
			__in_ecount(GradientStopCount) const GradientStop* pGradientStops, 
			UINT32 GradientStopCount, 
			__deref_out CGraphicsBrush** ppBrush 
			);

        __override virtual __checkReturn HRESULT GetDefaultBrush(
			DefaultBrush::Value Type, 
			__deref_out CGraphicsBrush** ppBrush 
			);

        __override virtual __checkReturn HRESULT DrawRectangle( 
			const RectF& Size, 
			__in const CGraphicsBrush* pBrush 
			);

        __override virtual __checkReturn HRESULT FillRectangle(
			const RectF& Size,
			__in const CGraphicsBrush* pBrush 
			);

        __override virtual __checkReturn HRESULT RenderTextLayout( 
			const Point2F& Origin, 
			__in const CTextLayout* pTextLayout, 
			__in const CGraphicsBrush* pBrush 
			);

        __override virtual __checkReturn HRESULT LoadBitmap( 
			__in const CBitmapSource* pSource, 
			__deref_out CBitmap** ppBitmap 
			);

        __override virtual __checkReturn HRESULT CreateBitmapBrush(
			__in const CBitmap* pBitmap,
			__deref_out CGraphicsBrush** CGraphicsBrush 
			);

        __override virtual __checkReturn HRESULT DrawGeometry( 
			__in const CGraphicsGeometry* pGeometry, 
			__in const CGraphicsBrush* pBrush, 
			FLOAT StrokeThickness = 1.0f 
			);

        __override virtual __checkReturn HRESULT FillGeometry( 
			__in const CGraphicsGeometry* pGeometry, 
			__in const CGraphicsBrush* pBrush
			);

        __override virtual __checkReturn HRESULT CreateLayer(
			__deref_out CLayer** ppLayer
			);

        __override virtual __checkReturn HRESULT PushLayer(
			__in const CLayer* pLayer, 
			const RectF& ClippingRect,
			FLOAT Opacity, 
			__in const CGraphicsGeometry* pClippingGeometry
			);

        __override virtual __checkReturn HRESULT PopLayer(
			);

    protected:
        COpenGLES20RenderTarget(
			);

        virtual ~COpenGLES20RenderTarget(
			);

        __checkReturn HRESULT Initialize(
			__in GLuint RenderBuffer,
			__in GLuint FrameBuffer,
			__in_opt COpenGLES20Context* pContext
			);

		__checkReturn HRESULT ApplyContext(
			);

        __checkReturn HRESULT CreateShader(
			const GLenum ShaderType,
			__in_z const CHAR* pShaderSource,
			__out GLuint* pShader
			);
    
        __checkReturn HRESULT LinkProgram(
            GLuint Program
            );
    
        __checkReturn HRESULT CreateTexture(
            UINT32 Width,
            UINT32 Height,
            __deref_out COpenGLES20Texture** ppTexture
            );
    
        __checkReturn HRESULT OnTesselatedGeometryBatch(
            __in COpenGLES20VertexBuffer* pVertexBuffer
            );
    
        __checkReturn HRESULT ApplyBrushTransformToSink(
            __in const CGraphicsBrush* pBrush
            );

		GLuint m_RenderBuffer;
		GLuint m_FrameBuffer;
		GLint m_Width;
		GLint m_Height;
		COpenGLES20Context* m_pContext;
        Matrix3X2F m_Transform;
        COpenGLES20VertexBuffer* m_pVertexBuffers[2];
        GLuint m_ShaderProgram;
        GLint m_PositionAttribute;
        GLint m_ColorAttribute;
        GLint m_TransformUniform;
        COpenGLES20TesselationSink* m_pTesselationSink;
};
