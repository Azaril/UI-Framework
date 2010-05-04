#pragma once

#include "FrameworkElement.h"
#include "Brush.h"

class CTextBlock : public CFrameworkElement
{
    public:
        DECLARE_FACTORY( CTextBlock );

        DECLARE_TYPE_WITH_BASE( TypeIndex::TextBlock, CFrameworkElement );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT SetValue( CProperty* pProperty, CObjectWithType* pValue );

        virtual HRESULT SetText( const WCHAR* pText );

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult );

    protected:
        CTextBlock();
        virtual ~CTextBlock();

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
        virtual HRESULT ArrangeInternal( SizeF Size );

        virtual HRESULT RenderTransformed( CRenderContext& Context );

        std::wstring m_Text;
        CTextFormat* m_TextFormat;
        CTextLayout* m_TextLayout;
        CBrush* m_TextBrush;
        CGraphicsBrush* m_TextGraphicsBrush;
};

template< >
struct ObjectTypeTraits< CTextBlock >
{
    static const TypeIndex::Value Type = TypeIndex::TextBlock;
};