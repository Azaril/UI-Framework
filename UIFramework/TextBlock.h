#pragma once

#include "FrameworkElement.h"
#include "Brush.h"

class CTextBlock : public CFrameworkElement
{
    public:
        DECLARE_FACTORY1( CTextBlock, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::TextBlock, CFrameworkElement );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT SetText( const WCHAR* pText );

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult );

        //
        // Properties
        //
        static CStaticProperty TextProperty;
        static CStaticProperty ForegroundProperty;

    protected:
        CTextBlock();
        virtual ~CTextBlock();

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
        virtual HRESULT ArrangeInternal( SizeF AvailableSize, SizeF& UsedSize );

        virtual HRESULT PreRenderInternal( CPreRenderContext& Context );
        virtual HRESULT RenderTransformed( CRenderContext& Context );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnTextChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnForegroundChanged );

        HRESULT OnTextChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnForegroundChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        HRESULT GetEffectiveText( CStringValue** ppText );
        HRESULT GetEffectiveForeground( CBrush** ppBrush );

        CTypedLayeredValue< CStringValue > m_Text;
        CTypedLayeredValue< CBrush > m_Foreground;
        CTextFormat* m_TextFormat;
        CTextLayout* m_TextLayout;
        CGraphicsBrush* m_ForegroundGraphicsBrush;
};

template< >
struct ObjectTypeTraits< CTextBlock >
{
    static const TypeIndex::Value Type = TypeIndex::TextBlock;
};