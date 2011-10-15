#pragma once

#include "RenderTarget.h"
#include "TextureAllocator.h"
#include "TextureAtlasPool.h"
#include "TesselationSink.h"
#include "GraphicsBrushBase.h"
#include "GeometryTesselationSink.h"
#include "TextureAtlasWithWhitePixel.h"

class UIFRAMEWORK_API CRenderTargetBase : public CRenderTarget,
                                          protected ITesselationBatchCallback
{
    public:
        __override virtual __checkReturn HRESULT BeginRendering(
			);

        __override virtual __checkReturn HRESULT EndRendering(
			);

        __override virtual __checkReturn HRESULT SetTransform( 
			const Matrix3X2F& Transform 
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
			__deref_out CGraphicsBrush** ppBrush 
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
        CRenderTargetBase(
			);

        virtual ~CRenderTargetBase(
			);

        __checkReturn HRESULT Initialize(
            __in CGeometryTesselationSink* pTesselationSink,
            __in ITextureAtlasPool* pTextureAtlasPool,
            __in CTextureAtlasView* pWhitePixelTexture
			);
    
        __checkReturn HRESULT ApplyBrush(
            __in const CGraphicsBrushBase* pBrush
            );
    
        __checkReturn HRESULT ApplyBrushToTesselationSink(
            __in const Matrix3X2F& textureToBrushTransform,
            __in const CGraphicsBrushBase* pBrush
            );

        virtual __checkReturn HRESULT ApplyContext(
            );

        virtual __checkReturn HRESULT BindTexture(
            __in ITexture* pTexture
            ) = 0;

        __checkReturn HRESULT Flush(
            );

        Matrix3X2F m_Transform;
        CGeometryTesselationSink* m_pTesselationSink;
        ITextureAtlasPool* m_pTextureAtlasPool;
        ITextureAtlasWithWhitePixel* m_pLastRenderedTextureAtlas;
        CTextureAtlasView* m_pDefaultWhitePixelTexture;
};
