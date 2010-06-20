#pragma once

#include "ContentControl.h"
#include "ScrollContentPresenter.h"

class CScrollViewer : public CContentControl
{
    public:
        DECLARE_FACTORY1( CScrollViewer, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::ScrollViewer, CContentControl );

        virtual HRESULT OnAttach( CUIAttachContext& Context );
        virtual HRESULT OnDetach( CUIDetachContext& Context );

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult );

        HRESULT LineUp();
        HRESULT LineDown();

    protected:
        CScrollViewer();
        virtual ~CScrollViewer();

        HRESULT Initialize( CProviders* pProviders );

        virtual void OnCommandExecuted( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );

        CScrollContentPresenter* m_ScrollPresenter;

    private:
        events::signals::connection m_RoutedCommandExecutedConnection;
};

template< >
struct ObjectTypeTraits< CScrollViewer >
{
    static const TypeIndex::Value Type = TypeIndex::ScrollViewer;
};