#pragma once

#include "RefCounted.h"
#include "Types.h"
#include "GraphicsBrush.h"
#include "TextLayout.h"
#include "BitmapSource.h"
#include "Bitmap.h"
#include "GraphicsGeometry.h"
#include "GradientStop.h"
#include "Layer.h"

namespace DefaultBrush
{
    enum Value
    {
        TextForeground
    };
}

class UIFRAMEWORK_API CRenderTarget : public CRefCountedObject
{
    public:
        virtual SizeF GetSize(
			) = 0;

        virtual __checkReturn HRESULT BeginRendering(
			) = 0;

        virtual __checkReturn HRESULT EndRendering(
			) = 0;

        virtual __checkReturn HRESULT SetTransform( 
			const Matrix3X2F& Transform 
			) = 0;

        virtual __checkReturn HRESULT Clear( 
			const ColorF& Color 
			) = 0;

        virtual __checkReturn HRESULT CreateSolidBrush( 
			const ColorF& Color, 
			__deref_out CGraphicsBrush** ppBrush 
			) = 0;

        virtual __checkReturn HRESULT CreateLinearGradientBrush( 
			const Point2F& StartPoint, 
			const Point2F& EndPoint, 
			__in_ecount(GradientStopCount) const GradientStop* pGradientStops,
			UINT32 GradientStopCount, 
			__deref_out CGraphicsBrush** ppBrush
			) = 0;

        virtual __checkReturn HRESULT GetDefaultBrush( 
			DefaultBrush::Value Type, 
			__deref_out CGraphicsBrush** ppBrush 
			) = 0;

        //TODO: Support border/stroke.
        virtual __checkReturn HRESULT DrawRectangle( 
			const RectF& Size,
			__in const CGraphicsBrush* pBrush 
			) = 0;

        virtual __checkReturn HRESULT FillRectangle( 
			const RectF& Size, 
			__in const CGraphicsBrush* pBrush 
			) = 0;

        virtual __checkReturn HRESULT RenderTextLayout( 
			const Point2F& Origin, 
			__in const CTextLayout* pTextLayout, 
			__in const CGraphicsBrush* pBrush 
			) = 0;

        virtual __checkReturn HRESULT LoadBitmap(
			__in const CBitmapSource* pSource, 
			__deref_out CBitmap** ppBitmap 
			) = 0;

        virtual __checkReturn HRESULT CreateBitmapBrush( 
			__in const CBitmap* pBitmap, 
			__deref_out CGraphicsBrush** pBrush 
			) = 0;

        virtual __checkReturn HRESULT DrawGeometry( 
			__in const CGraphicsGeometry* pGeometry, 
			__in const CGraphicsBrush* pBrush,
			FLOAT StrokeThickness = 1.0f 
			) = 0;

        virtual __checkReturn HRESULT FillGeometry(
			__in const CGraphicsGeometry* pGeometry, 
			__in const CGraphicsBrush* pBrush 
			) = 0;

        virtual __checkReturn HRESULT CreateLayer( 
			__deref_out CLayer** ppLayer 
			) = 0;

        virtual __checkReturn HRESULT PushLayer(
			__in const CLayer* pLayer, 
			const RectF& ClippingRect,
			FLOAT Opacity, 
			__in_opt const CGraphicsGeometry* pClippingGeometry 
			) = 0;

        virtual __checkReturn HRESULT PopLayer(
			) = 0;
};
