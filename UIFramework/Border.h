#pragma once

#include "Decorator.h"
#include "RectangleVisual.h"

class CBorder : public CDecorator
{
    public:
        DECLARE_FACTORY( CBorder );

        virtual TypeIndex::Value GetType() { return TypeIndex::Border; }
        virtual BOOL IsTypeOf( TypeIndex::Value Type ) { return Type == TypeIndex::Border || CDecorator::IsTypeOf(Type); }

        virtual HRESULT SetChild( CUIElement* pChild );

        HRESULT SetBackgroundBrush( CBrush* pBrush );

        HRESULT SetBorderThickness( const RectF& Border );
        HRESULT SetBorderBrush( CBrush* pBrush );

        HRESULT SetPadding( const RectF& Padding );

        virtual HRESULT Arrange( SizeF Size );

    protected:
        CBorder();
        virtual ~CBorder();

        HRESULT Initialize();
        HRESULT Finalize();

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );

        virtual HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        RectF m_BorderThickness;
        CRectangleVisual* m_BorderVisual;

        RectF m_Padding;
};
