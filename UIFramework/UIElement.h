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
class CFocusManager;
class CNamescope;

class CUIAttachContext
{
    public:
        CUIAttachContext() : m_Parent(NULL),
                             m_TemplateParent(NULL),
                             m_FocusManager(NULL),
                             m_Namescope(NULL)
        {
        }

        CUIAttachContext( const CUIAttachContext& Other ) : m_Parent(Other.m_Parent),
                                                            m_TemplateParent(Other.m_TemplateParent),
                                                            m_FocusManager(Other.m_FocusManager),
                                                            m_Namescope(Other.m_Namescope)
        {
            AddRefObject(m_Namescope);
        }

        CUIAttachContext( CUIElement* pParent, CUIElement* pTemplateParent, CFocusManager* pFocusManager, CNamescope* pNamescope ) : m_Parent(pParent),
                                                                                                                                     m_TemplateParent(pTemplateParent),
                                                                                                                                     m_FocusManager(pFocusManager),
                                                                                                                                     m_Namescope(pNamescope)
        {
            AddRefObject(m_Namescope);
        }

        ~CUIAttachContext()
        {
            ReleaseObject(m_Namescope);
        }
    
        CUIElement* GetParent()
        {
            return m_Parent;
        }

        CUIElement* GetTemplateParent()
        {
            return m_TemplateParent;
        }

        CFocusManager* GetFocusManager()
        {
            return m_FocusManager;
        }

        CNamescope* GetNamescope()
        {
            return m_Namescope;
        }

        CUIAttachContext& operator=(const CUIAttachContext& Other)
        {
            m_Parent = Other.m_Parent;
            m_TemplateParent = Other.m_TemplateParent;
            m_FocusManager = Other.m_FocusManager;
            m_Namescope = Other.m_Namescope;

            AddRefObject(m_Namescope);

            return *this;
        }

        void Reset()
        {
            m_Parent = NULL;
            m_TemplateParent = NULL;
            m_FocusManager = NULL;
            ReleaseObject(m_Namescope);
        }
    
    protected:
        CUIElement* m_Parent;
        CUIElement* m_TemplateParent;
        CFocusManager* m_FocusManager;
        CNamescope* m_Namescope;
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

typedef events::signal< void ( CObjectWithType*, CRoutedEventArgs* ) > RoutedEventSignal;
typedef RoutedEventSignal::slot_type RoutedEventHandler;

class CEventHandlerChain : public CRefCountedObject
{
    public:
        DECLARE_FACTORY1( CEventHandlerChain, CRoutedEvent* );

        CRoutedEvent* GetRoutedEvent();

        HRESULT AddHandler( const RoutedEventHandler& Handler, events::signals::connection* pConnection );

        BOOL HasHandlers();

        HRESULT RaiseEvent( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );

    protected:
        CEventHandlerChain();
        virtual ~CEventHandlerChain();

        HRESULT Initialize( CRoutedEvent* pRoutedEvent );

        CRoutedEvent* m_RoutedEvent;
        events::signal< void ( CObjectWithType*, CRoutedEventArgs* ) > m_Handlers;
};

class UIFRAMEWORK_API CUIElement : public CVisual
{
    public:
        DELEGATE_REFCOUNTING( CVisual );

        DECLARE_TYPE_WITH_BASE( TypeIndex::UIElement, CVisual );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );
        static HRESULT CreateEventInformation( CEventInformation** ppInformation );
        static HRESULT CreateCommandInformation( CCommandInformation** ppInformation );

        virtual HRESULT PreRender( CPreRenderContext& Context );
        virtual HRESULT Render( CRenderContext& Context );

        virtual HRESULT GetValue( CProperty* pProperty, CObjectWithType** ppValue );
        virtual HRESULT GetEffectiveValue( CProperty* pProperty, CObjectWithType** ppValue );
        virtual HRESULT ClearValue( CProperty* pProperty );

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
        CFocusManager* GetFocusManager();
        virtual CNamescope* GetNamescope();
        CProviders* GetProviders();
        CTypeConverter* GetTypeConverter();

        virtual HRESULT SetVisibility( Visibility::Value State );

        virtual HRESULT RaiseEvent( CRoutedEventArgs* pRoutedEventArgs );

        virtual HRESULT AddHandler( CRoutedEvent* pRoutedEvent, const RoutedEventHandler& Handler, events::signals::connection* pConnection );

        HRESULT Focus( BOOL* pSetFocus );
        BOOL IsFocusable();

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
        static CStaticProperty FocusableProperty;
        static CStaticProperty NamescopeProperty;

        //
        // Events
        //
        static CStaticRoutedEvent AttachedEvent;
        static CStaticRoutedEvent DetachedEvent;

        static CStaticRoutedEvent MouseButtonEvent;

        static CStaticRoutedEvent MouseDownEvent;
        static CStaticRoutedEvent MouseUpEvent;

        static CStaticRoutedEvent MouseLeftButtonDownEvent;
        static CStaticRoutedEvent MouseRightButtonDownEvent;
        static CStaticRoutedEvent MouseMiddleButtonDownEvent;

        static CStaticRoutedEvent MouseLeftButtonUpEvent;
        static CStaticRoutedEvent MouseRightButtonUpEvent;
        static CStaticRoutedEvent MouseMiddleButtonUpEvent;

        static CStaticRoutedEvent MouseMoveEvent;

        static CStaticRoutedEvent MouseEnterEvent;
        static CStaticRoutedEvent MouseLeaveEvent;

        static CStaticRoutedEvent PreviewGotFocusEvent;
        static CStaticRoutedEvent GotFocusEvent;

        static CStaticRoutedEvent PreviewLostFocusEvent;
        static CStaticRoutedEvent LostFocusEvent;

        static CStaticRoutedEvent KeyEvent;
        static CStaticRoutedEvent KeyDownEvent;
        static CStaticRoutedEvent KeyUpEvent;

        static CStaticRoutedEvent TextEvent;

    
    protected:
        CUIElement();
        virtual ~CUIElement();
    
        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT SetValueInternal( CProperty* pProperty, CObjectWithType* pValue );
        virtual HRESULT GetValueInternal( CProperty* pProperty, CObjectWithType** ppValue );

        virtual HRESULT PreRenderInternal( CPreRenderContext& Context );
        virtual HRESULT RenderInternal( CRenderContext& Context );

        virtual HRESULT GetClippingGeometry( CGeometry** ppGeometry );
        virtual HRESULT GetClippingRectangle( RectF* pClippingRectangle );

        virtual BOOL ShouldClipToLayout();

        HRESULT RequiresLayer( BOOL* pRequiresLayer );

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
        virtual HRESULT ArrangeInternal( SizeF AvailableSize, SizeF& UsedSize );

        virtual HRESULT NotifyParent( CUINotification* pNotification );

        virtual HRESULT OnUINotification( CUINotification* pNotification );

        virtual HRESULT OnChildMeasureInvalidated( CChildMeasureInvalidatedNotification* pNotification );
        virtual HRESULT OnChildArrangeInvalidated( CChildArrangeInvalidatedNotification* pNotification );

        virtual HRESULT InternalRaiseEvent( CRoutedEventArgs* pRoutedEventArgs );
        virtual HRESULT InternalRaiseBubbledEvent( CRoutedEventArgs* pRoutedEventArgs );
        virtual HRESULT InternalRaiseTunneledEvent( CRoutedEventArgs* pRoutedEventArgs );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        HRESULT GetEffectiveVisibility( Visibility::Value* pVisibility );
        HRESULT GetEffectiveMinimumWidth( FLOAT* pMinimumWidth );
        HRESULT GetEffectiveMinimumHeight( FLOAT* pMinimumHeight );
        HRESULT GetEffectiveMaximumWidth( FLOAT* pMaximumWidth );
        HRESULT GetEffectiveMaximumHeight( FLOAT* pMaximumHeight );
        HRESULT GetEffectiveHorizontalAlignment( HorizontalAlignment::Value* pAlignment );
        HRESULT GetEffectiveVerticalAlignment( VerticalAlignment::Value* pAlignment );
        HRESULT GetEffectiveMargin( RectF* pMargin );
        HRESULT GetEffectiveFocusable( BOOL* pFocusable );
        HRESULT GetEffectiveNamescope( CNamescope** ppNamescope );

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

        virtual void OnKey( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );

        HRESULT GetMinMaxSize( SizeF& MinimumSize, SizeF& MaximumSize );
        HRESULT ComputeAlignmentOffset( SizeF ClientSize, SizeF RenderSize, SizeF Offset );

        void CleanMeasure();
        void CleanArrange();

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
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnFocusableChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnNamescopeChanged );

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
        HRESULT OnFocusableChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnNamescopeChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
   
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
        CTypedLayeredValue< CBoolValue > m_Focusable;
        CTypedLocalLayeredValue< CNamescope > m_Namescope;

        BOOL m_MeasureDirty;
        BOOL m_ArrangeDirty;
        BOOL m_NotifiedParentMeasureDirty;
        BOOL m_NotifiedParentArrangeDirty;

    private:
        CProviders* m_Providers;
        BOOL m_Attached;

        SizeF m_LastMeasureSize;
        RectF m_LastArrangeBounds;
        SizeF m_DesiredSize;
        SizeF m_UnclippedDesiredSize;
        SizeF m_FinalSize;

        BOOL m_ClipToLayoutBounds;

        CLayer* m_Layer;

        CUIAttachContext m_Context;

        std::vector< CEventHandlerChain* > m_EventHandlers;

        events::signals::connection m_MouseButtonConnection;
        events::signals::connection m_MouseDownConnection;
        events::signals::connection m_MouseUpConnection;
        events::signals::connection m_MouseLeftButtonDownConnection;
        events::signals::connection m_MouseRightButtonDownConnection;
        events::signals::connection m_MouseMiddleButtonDownConnection;
        events::signals::connection m_MouseLeftButtonUpConnection;
        events::signals::connection m_MouseRightButtonUpConnection;
        events::signals::connection m_MouseMiddleButtonUpConnection;
        events::signals::connection m_MouseMoveConnection;
        events::signals::connection m_KeyConnection;
};

template< >
struct ObjectTypeTraits< CUIElement >
{
    static const TypeIndex::Value Type = TypeIndex::UIElement;
};