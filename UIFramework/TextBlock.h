#pragma once

#include "FrameworkElement.h"
#include "Brush.h"

class CTextBlock : public CFrameworkElement
{
    public:
        DECLARE_FACTORY( CTextBlock );

        virtual HRESULT SetText( const WCHAR* pText );

        virtual HRESULT Arrange( SizeF Size );

    protected:
        CTextBlock();
        virtual ~CTextBlock();

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );

        virtual HRESULT RenderTransformed( CRenderContext& Context );

        virtual HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        std::wstring m_Text;
        CTextFormat* m_TextFormat;
        CTextLayout* m_TextLayout;
        CBrush* m_TextBrush;
        CBrush* m_DefaultTextBrush;
};
