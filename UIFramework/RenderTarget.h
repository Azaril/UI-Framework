#pragma once

#include "RefCounted.h"
#include "Types.h"
#include "GraphicsBrush.h"
#include "TextLayout.h"
#include "BitmapSource.h"
#include "Bitmap.h"

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

        virtual HRESULT SetTransform( const Matrix3X2& Transform ) = 0;

        virtual HRESULT Clear( ColorF Color ) = 0;

        virtual HRESULT CreateSolidBrush( ColorF Color, CGraphicsBrush** ppBrush ) = 0;
        virtual HRESULT GetDefaultBrush( DefaultBrush::Value Type, CGraphicsBrush** ppBrush ) = 0;

        //TODO: Support border/stroke.
        virtual HRESULT DrawRectangle( const RectF& Size, CGraphicsBrush* pBrush ) = 0;
        virtual HRESULT FillRectangle( const RectF& Size, CGraphicsBrush* pBrush ) = 0;

        //virtual HRESULT CreateCompatibleRenderTarget() = 0;

        virtual HRESULT RenderTextLayout( const Point2F& Origin, CTextLayout* pTextLayout, CGraphicsBrush* pBrush ) = 0;

        virtual HRESULT LoadBitmap( CBitmapSource* pSource, CBitmap** ppBitmap ) = 0;

        virtual HRESULT CreateBitmapBrush( CBitmap* pBitmap, CGraphicsBrush** pBrush ) = 0;
};
