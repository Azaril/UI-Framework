#pragma once

#include "FrameworkElement.h"
#include "Brush.h"
#include "TextVisual.h"

class UIFRAMEWORK_API CTextBlock : public CFrameworkElement
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

        HRESULT Initialize( CProviders* pProviders );

        __override virtual __checkReturn HRESULT MeasureInternal( 
            const SizeF& AvailableSize,
            SizeF& DesiredSize
            );

        __override virtual __checkReturn HRESULT ArrangeInternal(
            const SizeF& AvailableSize,
            SizeF& UsedSize 
            );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnTextChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnForegroundChanged );

        HRESULT OnTextChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnForegroundChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        HRESULT GetTextLayout( CTextLayout** ppTextLayout );
        HRESULT InvalidateTextLayout();

        CLayeredValue m_Text;
        CLayeredValue m_Foreground;
        CTextFormat* m_TextFormat;
        CTextLayout* m_TextLayout;
        CTextVisual* m_TextVisual;
        bool m_TextLayoutDirty;
};

template< >
struct ObjectTypeTraits< CTextBlock >
{
    static const TypeIndex::Value Type = TypeIndex::TextBlock;
};