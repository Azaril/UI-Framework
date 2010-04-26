#pragma once

#include "Decorator.h"
#include "RectangleVisual.h"

class CBorder : public CDecorator
{
    public:
        DECLARE_FACTORY( CBorder );

        virtual HRESULT SetChild( CUIElement* pChild );

        HRESULT SetBackgroundBrush( CBrush* pBrush );

        HRESULT SetBorderThickness( const RectF& Border );
        HRESULT SetBorderBrush( CBrush* pBrush );

        virtual HRESULT Arrange( SizeF Size );

    protected:
        CBorder();
        virtual ~CBorder();

        HRESULT Initialize();
        HRESULT Finalize();

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );

        RectF m_BorderThickness;
        CRectangleVisual* m_BorderVisual;
};
