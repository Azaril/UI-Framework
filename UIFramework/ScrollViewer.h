#pragma once

#include "ContentControl.h"
#include "ScrollContentPresenter.h"

class CScrollViewer : public CContentControl,
                      public IScrollOwner
{
    public:
        DECLARE_FACTORY1( CScrollViewer, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::ScrollViewer, CContentControl );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT OnAttach( CUIAttachContext& Context );
        virtual HRESULT OnDetach( CUIDetachContext& Context );

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult );

        HRESULT LineUp();
        HRESULT LineDown();

        virtual HRESULT InvalidateScrollInformation();

        //
        // Properties
        //
        static CStaticProperty ExtentHeightProperty;
        static CStaticProperty ExtentWidthProperty;
        static CStaticProperty ViewportWidthProperty;
        static CStaticProperty ViewportHeightProperty;
        static CStaticProperty HorizontalOffsetProperty;
        static CStaticProperty VerticalOffsetProperty;
        static CStaticProperty ScrollableWidthProperty;
        static CStaticProperty ScrollableHeightProperty;

    protected:
        CScrollViewer();
        virtual ~CScrollViewer();

        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        virtual HRESULT PostTemplateApplied();
        virtual HRESULT PreTemplateRevoked();

        virtual void OnCommandExecuted( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );

        CScrollContentPresenter* m_ScrollPresenter;

        CTypedLocalLayeredValue< CScrollViewer, CFloatValue > m_ExtentHeight;
        CTypedLocalLayeredValue< CScrollViewer, CFloatValue > m_ExtentWidth;
        CTypedLocalLayeredValue< CScrollViewer, CFloatValue > m_ViewportWidth;
        CTypedLocalLayeredValue< CScrollViewer, CFloatValue > m_ViewportHeight;
        CTypedLocalLayeredValue< CScrollViewer, CFloatValue > m_HorizontalOffset;
        CTypedLocalLayeredValue< CScrollViewer, CFloatValue > m_VerticalOffset;
        CTypedLocalLayeredValue< CScrollViewer, CFloatValue > m_ScrollableWidth;
        CTypedLocalLayeredValue< CScrollViewer, CFloatValue > m_ScrollableHeight;

    private:
        events::signals::connection m_RoutedCommandExecutedConnection;
};

template< >
struct ObjectTypeTraits< CScrollViewer >
{
    static const TypeIndex::Value Type = TypeIndex::ScrollViewer;
};