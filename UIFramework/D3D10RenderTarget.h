#pragma once

#if defined(FRAMEWORK_D3D10)

#include <d3d10.h>

#include "RenderTarget.h"
#include "TextureAllocator.h"
#include "TextureAtlasPool.h"
#include "RenderTargetBase.h"
#include "D3D10VertexBuffer.h"
#include "StagingTextureCallback.h"

class UIFRAMEWORK_API CD3D10RenderTarget : public CRenderTargetBase
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
        CD3D10RenderTarget(
			);

        virtual ~CD3D10RenderTarget(
			);

        __checkReturn HRESULT Initialize(
            __in ID3D10Device* pDevice,
            __in ID3D10RenderTargetView* pRenderTargetView,
            __in CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >* pTextureAtlasPool
			);

        __override virtual __checkReturn HRESULT BindTexture(
            __in ITexture* pTexture
            );

        __override virtual __checkReturn HRESULT BindMask(
            __in ITexture* pTexture
            );

        __override virtual __checkReturn HRESULT OnTesselatedGeometryBatch(
            __in IVertexBuffer* pVertexBuffer
            );

        ID3D10Device* m_pDevice;
        CD3D10VertexBuffer* m_pVertexBuffers[2];
        ID3D10VertexShader* m_pVertexShader;
        ID3D10PixelShader* m_pPixelShader;
        ID3D10InputLayout* m_pInputLayout;
        ID3D10Buffer* m_pTransformBuffer;
        ID3D10RasterizerState* m_pRasterizerState;
        ID3D10SamplerState* m_pSamplerState;
        ID3D10BlendState* m_pBlendState;
        ID3D10DepthStencilState* m_pDepthStencilState;

    private:
        ID3D10RenderTargetView* m_pRenderTargetView;
};

#endif