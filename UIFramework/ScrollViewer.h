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
        static CStaticProperty ComputedHorizontalScrollBarVisibilityProperty;
        static CStaticProperty ComputedVerticalScrollBarVisibilityProperty;

    protected:
        CScrollViewer();
        virtual ~CScrollViewer();

        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        __override virtual __checkReturn HRESULT MeasureInternal( 
            const SizeF& AvailableSize,
            SizeF& DesiredSize
            );

        virtual HRESULT PostTemplateApplied();
        virtual HRESULT PreTemplateRevoked();

        virtual void OnCommandExecuted( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );

        HRESULT SetComputedVerticalScrollBarVisibility( Visibility::Value ScrollVisibility );
        HRESULT SetComputedHorizontalScrollBarVisibility( Visibility::Value ScrollVisibility );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnExtentHeightChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnExtentWidthChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnViewportWidthChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnViewportHeightChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnHorizontalOffsetChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnVerticalOffsetChanged );

        HRESULT OnExtentHeightChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnExtentWidthChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnViewportWidthChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnViewportHeightChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnHorizontalOffsetChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnVerticalOffsetChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        CScrollContentPresenter* m_ScrollPresenter;

        CLayeredValue m_ExtentHeight;
        CLayeredValue m_ExtentWidth;
        CLayeredValue m_ViewportWidth;
        CLayeredValue m_ViewportHeight;
        CLayeredValue m_HorizontalOffset;
        CLayeredValue m_VerticalOffset;
        CLayeredValue m_ScrollableWidth;
        CLayeredValue m_ScrollableHeight;
        CLayeredValue m_ComputedHorizontalScrollBarVisibility;
        CLayeredValue m_ComputedVerticalScrollBarVisibility;

    private:
        bool m_InMeasure;
        bool m_InInvalidate;
        events::signals::connection m_RoutedCommandExecutedConnection;
};

template< >
struct ObjectTypeTraits< CScrollViewer >
{
    static const TypeIndex::Value Type = TypeIndex::ScrollViewer;
};