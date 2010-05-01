#pragma once

#include "Decorator.h"
#include "RectangleVisual.h"

class CBorder : public CDecorator
{
    public:
        DECLARE_FACTORY( CBorder );

        virtual TypeIndex::Value GetType() { return TypeIndex::Border; }
        virtual BOOL IsTypeOf( TypeIndex::Value Type ) { return Type == TypeIndex::Border || CDecorator::IsTypeOf(Type); }

        virtual HRESULT SetValue( CProperty* pProperty, CObjectWithType* pValue );

        virtual HRESULT SetChild( CUIElement* pChild );

        HRESULT SetBackground( CBrush* pBrush );

        HRESULT SetBorderThickness( const RectF& Border );
        HRESULT SetBorder( CBrush* pBrush );

        HRESULT SetPadding( const RectF& Padding );

    protected:
        CBorder();
        virtual ~CBorder();

        HRESULT Initialize();
        HRESULT Finalize();

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
        virtual HRESULT ArrangeInternal( SizeF Size );

        virtual HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        RectF m_BorderThickness;
        CRectangleVisual* m_BorderVisual;

        RectF m_Padding;
};
