#pragma once

#include <d3d9.h>

#include "RenderTarget.h"
#include "TextureAllocator.h"
#include "TextureAtlasPool.h"
#include "RenderTargetBase.h"
#include "D3D9VertexBuffer.h"

class UIFRAMEWORK_API CD3D9RenderTarget : public CRenderTargetBase
{
    public:
        /*
        __override virtual SizeF GetSize(
			);
            */

        __override virtual __checkReturn HRESULT BeginRendering(
			);

        __override virtual __checkReturn HRESULT EndRendering(
			);

        /*
        __override virtual __checkReturn HRESULT SetTransform( 
			const Matrix3X2F& Transform 
			);
            */

        __override virtual __checkReturn HRESULT Clear( 
			const ColorF& Color 
			);

        /*
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
            */

    protected:
        CD3D9RenderTarget(
			);

        virtual ~CD3D9RenderTarget(
			);

        __checkReturn HRESULT Initialize(
            __in IDirect3DDevice9* pDevice,
            __in CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >* pTextureAtlasPool
			);

        __override virtual __checkReturn HRESULT BindTexture(
            __in ITexture* pTexture
            );

        __override virtual __checkReturn HRESULT OnTesselatedGeometryBatch(
            __in IVertexBuffer* pVertexBuffer
            );

        IDirect3DDevice9* m_pDevice;
        CD3D9VertexBuffer* m_pVertexBuffers[2];
        IDirect3DVertexDeclaration9* m_pVertexDeclaration;
        IDirect3DPixelShader9* m_pPixelShader;
        IDirect3DVertexShader9* m_pVertexShader;
};
