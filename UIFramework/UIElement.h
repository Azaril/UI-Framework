#pragma once

#include "Visual.h"
#include "RectangleVisual.h"
#include "RenderTarget.h"
#include "PropertyObject.h"
#include "TypeIndex.h"
#include "Enums.h"
#include "RoutedEventArgs.h"
#include "StaticRoutedEvent.h"
#include "Signals.h"
#include "Namescope.h"

class CUIElement;

class CUIAttachContext
{
    public:
        CUIAttachContext() : m_Parent(NULL)
        {
        }

        CUIAttachContext( CUIElement* pParent ) : m_Parent(pParent)
        {
        }
    
        CUIElement* GetParent()
        {
            return m_Parent;
        }

        void Reset()
        {
            m_Parent = NULL;
        }
    
    protected:
        CUIElement* m_Parent;
};

class CUIDetachContext
{
    public:
        CUIDetachContext() : m_Parent(NULL)
        {
        }

        CUIDetachContext( CUIElement* pParent ) : m_Parent(pParent)
        {
        }
    
        CUIElement* GetParent()
        {
            return m_Parent;
        }

        void Reset()
        {
            m_Parent = NULL;
        }
        
    protected:
        CUIElement* m_Parent;
};

namespace UINotification
{
    enum Value
    {
        ChildMeasureInvalidated,
        ChildArrangeInvalidated
    };
}

class CUINotification : public CRefCountedObject
{
    public:
        virtual UINotification::Value GetType() = 0;
};

template< UINotification::Value Val >
class CParameterlessUINotification : public CUINotification
{
    public: 
        DECLARE_FACTORY( CParameterlessUINotification< Val > );

        virtual UINotification::Value GetType()
        {
            return Val;
        }

    protected:
        HRESULT Initialize()
        {
            return S_OK;
        }
};

typedef CParameterlessUINotification< UINotification::ChildMeasureInvalidated > CChildMeasureInvalidatedNotification;
typedef CParameterlessUINotification< UINotification::ChildArrangeInvalidated > CChildArrangeInvalidatedNotification;

typedef signal< void ( CObjectWithType*, CRoutedEventArgs* ) > RoutedEventSignal;
typedef RoutedEventSignal::slot_type RoutedEventHandler;

class CEventHandlerChain : public CRefCountedObject
{
    public:
        DECLARE_FACTORY1( CEventHandlerChain, CRoutedEvent* );

        CRoutedEvent* GetRoutedEvent();

        HRESULT AddHandler( const RoutedEventHandler& Handler, connection* pConnection );

        BOOL HasHandlers();

        HRESULT RaiseEvent( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );

    protected:
        CEventHandlerChain();
        virtual ~CEventHandlerChain();

        HRESULT Initialize( CRoutedEvent* pRoutedEvent );

        CRoutedEvent* m_RoutedEvent;
        signal< void ( CObjectWithType*, CRoutedEventArgs* ) > m_Handlers;
};

class CUIElement : public CVisual
{
    public:
        DELEGATE_REFCOUNTING( CVisual );

        DECLARE_TYPE_WITH_BASE( TypeIndex::UIElement, CVisual );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT PreRender( CPreRenderContext& Context );
        virtual HRESULT Render( CRenderContext& Context );

        virtual HRESULT SetValue( CProperty* pProperty, CObjectWithType* pValue );
        virtual HRESULT GetValue( CProperty* pProperty, CObjectWithType** ppValue );

        virtual HRESULT OnAttach( CUIAttachContext& Context );
        virtual HRESULT OnDetach( CUIDetachContext& Context );

        BOOL IsAttached();

        HRESULT SetSize( SizeF Size );

        virtual HRESULT Measure( SizeF Size );
        virtual SizeF GetDesiredSize();

        virtual HRESULT Arrange( SizeF Size );

        virtual HRESULT InvalidateMeasure();
        virtual HRESULT InvalidateArrange();

        BOOL IsMeasureDirty();
        BOOL IsArrangeDirty();

        virtual CUIElement* GetParent();

        virtual HRESULT SetVisibility( Visibility::Value State );

        virtual HRESULT RaiseEvent( CRoutedEventArgs* pRoutedEventArgs );

        virtual HRESULT AddHandler( CRoutedEvent* pRoutedEvent, const RoutedEventHandler& Handler, connection* pConnection );
        /*virtual HRESULT RemoveHandler( CRoutedEvent* pRoutedEvent, const RoutedEventHandler& Handler );*/

        static CStaticRoutedEvent< RoutingStrategy::Bubbling > MouseButtonEvent;

        static CStaticRoutedEvent< RoutingStrategy::Direct > MouseDownEvent;
        static CStaticRoutedEvent< RoutingStrategy::Direct > MouseUpEvent;

        static CStaticRoutedEvent< RoutingStrategy::Direct > MouseLeftButtonDownEvent;
        static CStaticRoutedEvent< RoutingStrategy::Direct > MouseRightButtonDownEvent;
        static CStaticRoutedEvent< RoutingStrategy::Direct > MouseMiddleButtonDownEvent;

        static CStaticRoutedEvent< RoutingStrategy::Direct > MouseLeftButtonUpEvent;
        static CStaticRoutedEvent< RoutingStrategy::Direct > MouseRightButtonUpEvent;
        static CStaticRoutedEvent< RoutingStrategy::Direct > MouseMiddleButtonUpEvent;

        static CStaticRoutedEvent< RoutingStrategy::Bubbling > MouseMoveEvent;
    
    protected:
        CUIElement();
        virtual ~CUIElement();
    
        HRESULT Initialize();

        virtual HRESULT PreRenderInternal( CPreRenderContext& Context );
        virtual HRESULT RenderInternal( CRenderContext& Context );

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
        virtual HRESULT ArrangeInternal( SizeF AvailableSize );

        virtual HRESULT NotifyParent( CUINotification* pNotification );

        virtual HRESULT OnNotification( CUINotification* pNotification );

        virtual HRESULT OnChildMeasureInvalidated( CChildMeasureInvalidatedNotification* pNotification );
        virtual HRESULT OnChildArrangeInvalidated( CChildArrangeInvalidatedNotification* pNotification );

        virtual HRESULT InternalSetVisibility( Visibility::Value State );

        virtual HRESULT InternalRaiseEvent( CRoutedEventArgs* pRoutedEventArgs );
        virtual HRESULT InternalRaiseBubbledEvent( CRoutedEventArgs* pRoutedEventArgs );

        SizeF GetFinalSize();

        HRESULT InternalSetWidth( FLOAT Width );
        HRESULT InternalSetHeight( FLOAT Height );

        HRESULT InternalSetMinimumWidth( FLOAT Width );
        HRESULT InternalSetMinimumHeight( FLOAT Height );

        HRESULT InternalSetMaximumWidth( FLOAT Width );
        HRESULT InternalSetMaximumHeight( FLOAT Height );

        virtual void OnMouseButton( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );

        virtual void OnMouseDown( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );
        virtual void OnMouseUp( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );

        virtual void OnMouseLeftButtonDown( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );
        virtual void OnMouseRightButtonDown( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );
        virtual void OnMouseMiddleButtonDown( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );

        virtual void OnMouseLeftButtonUp( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );
        virtual void OnMouseRightButtonUp( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );
        virtual void OnMouseMiddleButtonUp( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );

        virtual void OnMouseMove( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );
   
        BOOL m_Attached;

        SizeF m_Size;
        SizeF m_MinimumSize;
        SizeF m_MaximumSize;

        BOOL m_MeasureDirty;
        BOOL m_ArrangeDirty;

        CPropertyInformation* m_PropertyInformation;

        Visibility::Value m_Visibility;

    private:
        SizeF m_LastMeasureSize;
        SizeF m_DesiredSize;
        SizeF m_FinalSize;

        CUIAttachContext m_Context;

        std::vector< CEventHandlerChain* > m_EventHandlers;

        connection m_MouseButtonConnection;
        connection m_MouseDownConnection;
        connection m_MouseUpConnection;
        connection m_MouseLeftButtonDownConnection;
        connection m_MouseRightButtonDownConnection;
        connection m_MouseMiddleButtonDownConnection;
        connection m_MouseLeftButtonUpConnection;
        connection m_MouseRightButtonUpConnection;
        connection m_MouseMiddleButtonUpConnection;
        connection m_MouseMoveConnection;
};

template< >
struct ObjectTypeTraits< CUIElement >
{
    static const TypeIndex::Value Type = TypeIndex::UIElement;
};