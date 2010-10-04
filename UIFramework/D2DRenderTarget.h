#pragma once

#include "RenderTarget.h"

#include <d2d1.h>
#include <d2d1helper.h>

class UIFRAMEWORK_API CD2DRenderTarget : public CRenderTarget
{
    public:
        virtual SizeF GetSize();

        virtual HRESULT BeginRendering();
        virtual HRESULT EndRendering();

        virtual HRESULT SetTransform( const Matrix3X2F& Transform );

        virtual HRESULT Clear( ColorF Color );

        virtual HRESULT CreateSolidBrush( ColorF Color, CGraphicsBrush** ppBrush );
        virtual HRESULT CreateLinearGradientBrush( const Point2F& StartPoint, const Point2F& EndPoint, GradientStop* pGradientStops, UINT32 GradientStopCount, CGraphicsBrush** ppBrush );
        virtual HRESULT GetDefaultBrush( DefaultBrush::Value Type, CGraphicsBrush** ppBrush );

        virtual HRESULT DrawRectangle( const RectF& Size, CGraphicsBrush* pBrush );
        virtual HRESULT FillRectangle( const RectF& Size, CGraphicsBrush* pBrush );

        virtual HRESULT RenderTextLayout( const Point2F& Origin, CTextLayout* pTextLayout, CGraphicsBrush* pBrush );

        virtual HRESULT LoadBitmap( CBitmapSource* pSource, CBitmap** ppBitmap );

        virtual HRESULT CreateBitmapBrush( CBitmap* pBitmap, CGraphicsBrush** CGraphicsBrush );

        virtual HRESULT DrawGeometry( CGeometry* pGeometry, CGraphicsBrush* pBrush, FLOAT StrokeThickness = 1.0f );
        virtual HRESULT FillGeometry( CGeometry* pGeometry, CGraphicsBrush* pBrush );

        virtual HRESULT CreateLayer( CLayer** ppLayer );

        virtual HRESULT PushLayer( CLayer* pLayer, const RectF& ClippingRect, FLOAT Opacity, CGeometry* pClippingGeometry );
        virtual HRESULT PopLayer();

    protected:
        CD2DRenderTarget();
        virtual ~CD2DRenderTarget();

        HRESULT Initialize( ID2D1RenderTarget* pRenderTarget );

        HRESULT UnwrapGeometry( CGeometry* pGeometry, ID2D1Geometry** ppD2DGeometry );

        ID2D1RenderTarget* m_RenderTarget;
};
