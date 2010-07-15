#pragma once

#include "RefCounted.h"
#include "Types.h"
#include "GraphicsBrush.h"
#include "TextLayout.h"
#include "BitmapSource.h"
#include "Bitmap.h"
#include "Geometry.h"
#include "GradientStop.h"
#include "Layer.h"

namespace DefaultBrush
{
    enum Value
    {
        TextForeground
    };
}

class CRenderTarget : public CRefCountedObject
{
    public:
        virtual SizeF GetSize() = 0;

        virtual HRESULT BeginRendering() = 0;
        virtual HRESULT EndRendering() = 0;

        virtual HRESULT SetTransform( const Matrix3X2F& Transform ) = 0;

        virtual HRESULT Clear( ColorF Color ) = 0;

        virtual HRESULT CreateSolidBrush( ColorF Color, CGraphicsBrush** ppBrush ) = 0;
        virtual HRESULT CreateLinearGradientBrush( const Point2F& StartPoint, const Point2F& EndPoint, GradientStop* pGradientStops, UINT32 GradientStopCount, CGraphicsBrush** ppBrush ) = 0;
        virtual HRESULT GetDefaultBrush( DefaultBrush::Value Type, CGraphicsBrush** ppBrush ) = 0;

        //TODO: Support border/stroke.
        virtual HRESULT DrawRectangle( const RectF& Size, CGraphicsBrush* pBrush ) = 0;
        virtual HRESULT FillRectangle( const RectF& Size, CGraphicsBrush* pBrush ) = 0;

        //virtual HRESULT CreateCompatibleRenderTarget() = 0;

        virtual HRESULT RenderTextLayout( const Point2F& Origin, CTextLayout* pTextLayout, CGraphicsBrush* pBrush ) = 0;

        virtual HRESULT LoadBitmap( CBitmapSource* pSource, CBitmap** ppBitmap ) = 0;

        virtual HRESULT CreateBitmapBrush( CBitmap* pBitmap, CGraphicsBrush** pBrush ) = 0;

        virtual HRESULT DrawGeometry( CGeometry* pGeometry, CGraphicsBrush* pBrush, FLOAT StrokeThickness = 1.0f ) = 0;
        virtual HRESULT FillGeometry( CGeometry* pGeometry, CGraphicsBrush* pBrush ) = 0;

        virtual HRESULT CreateLayer( CLayer** ppLayer ) = 0;

        virtual HRESULT PushLayer( CLayer* pLayer, const RectF& ClippingRect, CGeometry* pClippingGeometry ) = 0;
        virtual HRESULT PopLayer() = 0;
};
