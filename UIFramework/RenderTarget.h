#pragma once

#include "RefCounted.h"
#include "Types.h"
#include "Brush.h"
#include "TextLayout.h"

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

        virtual HRESULT CreateSolidBrush( ColorF Color, CBrush** ppBrush ) = 0;
        virtual HRESULT GetDefaultBrush( DefaultBrush::Value Type, CBrush** ppBrush ) = 0;

        //TODO: Support border/stroke.
        virtual HRESULT DrawRectangle( const RectF& Size, CBrush* pBrush ) = 0;
        virtual HRESULT FillRectangle( const RectF& Size, CBrush* pBrush ) = 0;

        //virtual HRESULT CreateCompatibleRenderTarget() = 0;

        virtual HRESULT RenderTextLayout( const Point2F& Origin, CTextLayout* pTextLayout, CBrush* pBrush ) = 0;
};
