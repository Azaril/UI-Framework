#pragma once

#include "FrameworkElement.h"
#include "Brush.h"

class CTextBlock : public CFrameworkElement
{
    public:
        DECLARE_FACTORY( CTextBlock );

        virtual TypeIndex::Value GetType() { return TypeIndex::TextBlock; }
        virtual BOOL IsTypeOf( TypeIndex::Value Type ) { return Type == TypeIndex::TextBlock || CFrameworkElement::IsTypeOf(Type); }

        virtual HRESULT SetValue( CProperty* pProperty, CObjectWithType* pValue );

        virtual HRESULT SetText( const WCHAR* pText );

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult );

    protected:
        CTextBlock();
        virtual ~CTextBlock();

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
        virtual HRESULT ArrangeInternal( SizeF Size );

        virtual HRESULT RenderTransformed( CRenderContext& Context );

        virtual HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        std::wstring m_Text;
        CTextFormat* m_TextFormat;
        CTextLayout* m_TextLayout;
        CBrush* m_TextBrush;
        CGraphicsBrush* m_TextGraphicsBrush;
};
