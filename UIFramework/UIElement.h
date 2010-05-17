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
#include "ClassResolver.h"
#include "TypeConverter.h"
#include "Providers.h"
#include "LayeredValue.h"
#include "BasicTypes.h"
#include "StaticPropertyInformation.h"

class CUIElement;

class CUIAttachContext
{
    public:
        CUIAttachContext() : m_Parent(NULL),
                             m_TemplateParent(NULL)
        {
        }

        CUIAttachContext( CUIElement* pParent, CUIElement* pTemplateParent ) : m_Parent(pParent),
                                                                               m_TemplateParent(pTemplateParent)
        {
        }
    
        CUIElement* GetParent()
        {
            return m_Parent;
        }

        CUIElement* GetTemplateParent()
        {
            return m_TemplateParent;
        }

        void Reset()
        {
            m_Parent = NULL;
            m_TemplateParent = NULL;
        }
    
    protected:
        CUIElement* m_Parent;
        CUIElement* m_TemplateParent;
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
        static HRESULT CreateEventInformation( CEventInformation** ppInformation );

        virtual HRESULT PreRender( CPreRenderContext& Context );
        virtual HRESULT Render( CRenderContext& Context );

        virtual HRESULT GetValue( CProperty* pProperty, CObjectWithType** ppValue );
        virtual HRESULT GetEffectiveValue( CProperty* pProperty, CObjectWithType** ppValue );

        virtual HRESULT OnAttach( CUIAttachContext& Context );
        virtual HRESULT OnDetach( CUIDetachContext& Context );

        BOOL IsAttached();

        HRESULT SetSize( SizeF Size );

        virtual HRESULT Measure( SizeF Size );
        virtual SizeF GetDesiredSize();

        virtual HRESULT Arrange( RectF Bounds );
        SizeF GetFinalSize();

        virtual HRESULT InvalidateMeasure();
        virtual HRESULT InvalidateArrange();

        BOOL IsMeasureDirty();
        BOOL IsArrangeDirty();

        virtual CUIElement* GetParent();
        virtual CUIElement* GetTemplateParent();
        CProviders* GetProviders();
        CTypeConverter* GetTypeConverter();

        virtual HRESULT SetVisibility( Visibility::Value State );

        virtual HRESULT RaiseEvent( CRoutedEventArgs* pRoutedEventArgs );

        virtual HRESULT AddHandler( CRoutedEvent* pRoutedEvent, const RoutedEventHandler& Handler, connection* pConnection );

        //
        // Properties
        //
        static CStaticProperty WidthProperty;
        static CStaticProperty HeightProperty;
        static CStaticProperty MinimumWidthProperty;
        static CStaticProperty MinimumHeightProperty;
        static CStaticProperty MaximumWidthProperty;
        static CStaticProperty MaximumHeightProperty;
        static CStaticProperty VisibilityProperty;
        static CStaticProperty HorizontalAlignmentProperty;
        static CStaticProperty VerticalAlignmentProperty;
        static CStaticProperty MarginProperty;

        //
        // Events
        //
        static CStaticRoutedEvent< RoutingStrategy::Direct > AttachedEvent;
        static CStaticRoutedEvent< RoutingStrategy::Direct > DetachedEvent;

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
    
        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT SetValueInternal( CProperty* pProperty, CObjectWithType* pValue );
        virtual HRESULT GetValueInternal( CProperty* pProperty, CObjectWithType** ppValue );

        virtual HRESULT PreRenderInternal( CPreRenderContext& Context );
        virtual HRESULT RenderInternal( CRenderContext& Context );

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
        virtual HRESULT ArrangeInternal( SizeF AvailableSize, SizeF& UsedSize );

        virtual HRESULT NotifyParent( CUINotification* pNotification );

        virtual HRESULT OnUINotification( CUINotification* pNotification );

        virtual HRESULT OnChildMeasureInvalidated( CChildMeasureInvalidatedNotification* pNotification );
        virtual HRESULT OnChildArrangeInvalidated( CChildArrangeInvalidatedNotification* pNotification );

        virtual HRESULT InternalRaiseEvent( CRoutedEventArgs* pRoutedEventArgs );
        virtual HRESULT InternalRaiseBubbledEvent( CRoutedEventArgs* pRoutedEventArgs );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        HRESULT GetEffectiveVisibility( Visibility::Value* pVisibility );
        HRESULT GetEffectiveMinimumWidth( FLOAT* pMinimumWidth );
        HRESULT GetEffectiveMinimumHeight( FLOAT* pMinimumHeight );
        HRESULT GetEffectiveMaximumWidth( FLOAT* pMaximumWidth );
        HRESULT GetEffectiveMaximumHeight( FLOAT* pMaximumHeight );
        HRESULT GetEffectiveHorizontalAlignment( HorizontalAlignment::Value* pAlignment );
        HRESULT GetEffectiveVerticalAlignment( VerticalAlignment::Value* pAlignment );
        HRESULT GetEffectiveMargin( RectF* pMargin );

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

        HRESULT GetMinMaxSize( SizeF& MinimumSize, SizeF& MaximumSize );
        HRESULT ComputeAlignmentOffset( SizeF ClientSize, SizeF RenderSize, SizeF Offset );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnWidthChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnHeightChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnMinimumWidthChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnMinimumHeightChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnMaximumWidthChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnMaximumHeightChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnVisibilityChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnHorizontalAlignmentChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnVerticalAlignmentChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnMarginChanged );

        HRESULT OnWidthChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnHeightChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnMinimumWidthChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnMinimumHeightChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnMaximumWidthChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnMaximumHeightChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnVisibilityChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnHorizontalAlignmentChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnVerticalAlignmentChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnMarginChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
   
        CTypedLayeredValue< CFloatValue > m_Width;
        CTypedLayeredValue< CFloatValue > m_Height;
        CTypedLayeredValue< CFloatValue > m_MinimumWidth;
        CTypedLayeredValue< CFloatValue > m_MinimumHeight;
        CTypedLayeredValue< CFloatValue > m_MaximumWidth;
        CTypedLayeredValue< CFloatValue > m_MaximumHeight;
        CTypedLayeredValue< CVisibilityValue > m_Visibility;
        CTypedLayeredValue< CVerticalAlignmentValue > m_VerticalAlignment;
        CTypedLayeredValue< CHorizontalAlignmentValue > m_HorizontalAlignment;
        CTypedLayeredValue< CRectFValue > m_Margin;

        BOOL m_MeasureDirty;
        BOOL m_ArrangeDirty;

        CPropertyInformation* m_PropertyInformation;

    private:
        CProviders* m_Providers;
        BOOL m_Attached;

        SizeF m_LastMeasureSize;
        RectF m_LastArrangeBounds;
        SizeF m_DesiredSize;
        SizeF m_UnclippedDesiredSize;
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