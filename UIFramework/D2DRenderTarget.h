#pragma once

#include "RenderTarget.h"

#include <d2d1.h>
#include <d2d1helper.h>

class CD2DRenderTarget : public CRenderTarget
{
    public:
        virtual SizeF GetSize();

        virtual HRESULT BeginRendering();
        virtual HRESULT EndRendering();

        virtual HRESULT SetTransform( const Matrix3X2& Transform );

        virtual HRESULT Clear( ColorF Color );

        virtual HRESULT CreateSolidBrush( ColorF Color, CBrush** ppBrush );
        virtual HRESULT GetDefaultBrush( DefaultBrush::Value Type, CBrush** ppBrush );

        virtual HRESULT DrawRectangle( const RectF& Size, CBrush* pBrush );
        virtual HRESULT FillRectangle( const RectF& Size, CBrush* pBrush );

        virtual HRESULT RenderTextLayout( const Point2F& Origin, CTextLayout* pTextLayout, CBrush* pBrush );

    protected:
        CD2DRenderTarget();
        virtual ~CD2DRenderTarget();

        HRESULT Initialize( ID2D1RenderTarget* pRenderTarget );

        ID2D1RenderTarget* m_RenderTarget;
};
