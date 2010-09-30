#pragma once

#include "Control.h"

class CThumb : public CControl
{
    public:
        DECLARE_FACTORY1( CThumb, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::Thumb, CControl );

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult );

        //
        // Events
        //
        static CStaticRoutedEvent DragStartedEvent;
        static CStaticRoutedEvent DragDeltaEvent;
        static CStaticRoutedEvent DragCompletedEvent;

    protected:
        CThumb();
        virtual ~CThumb();

        HRESULT Initialize( CProviders* pProviders );

        virtual void OnMouseLeftButtonDown( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );
        virtual void OnMouseMove( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );
        virtual void OnMouseLeftButtonUp( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );

        HRESULT StartDragging( Point2F Location );
        HRESULT UpdateDrag( Point2F Location );
        HRESULT StopDragging( Point2F Location );

        BOOL m_Dragging;
        Point2F m_LastMouseLocation;
};

template< >
struct ObjectTypeTraits< CThumb >
{
    static const TypeIndex::Value Type = TypeIndex::Thumb;
};