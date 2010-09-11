#pragma once

#include "FrameworkElement.h"
#include "TextVisual.h"

class UIFRAMEWORK_API CTextHost : public CFrameworkElement
{
    public:
        DECLARE_FACTORY1( CTextHost, CProviders* );
        DECLARE_FACTORY2( CTextHost, CProviders*, CTextLayout* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::TextHost, CFrameworkElement );

        HRESULT SetTextLayout( CTextLayout* pTextLayout );

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult );

    protected:
        CTextHost();
        virtual ~CTextHost();

        HRESULT Initialize( CProviders* pProviders, CTextLayout* pLayout = NULL );

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
        virtual HRESULT ArrangeInternal( SizeF AvailableSize, SizeF& UsedSize );

        CTextFormat* m_TextFormat;
        CTextLayout* m_TextLayout;
        CTextVisual* m_TextVisual;
};
