#pragma once

#include "Visual.h"
#include "RectangleVisual.h"
#include "RenderTarget.h"
#include "PropertyObject.h"
#include "TypeIndex.h"
#include "Enums.h"

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

class CUIElement : public CVisual
{
    public:
        DELEGATE_REFCOUNTING( CVisual );

        virtual TypeIndex::Value GetType() { return TypeIndex::UIElement; }
        virtual BOOL IsTypeOf( TypeIndex::Value Type ) { return Type == TypeIndex::UIElement || CVisual::IsTypeOf(Type); }

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

        virtual HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT InternalSetVisibility( Visibility::Value State );

        SizeF GetFinalSize();

        HRESULT InternalSetWidth( FLOAT Width );
        HRESULT InternalSetHeight( FLOAT Height );
   
        BOOL m_Attached;
        CUIAttachContext m_Context;

        SizeF m_Size;

        BOOL m_MeasureDirty;
        BOOL m_ArrangeDirty;

        CPropertyInformation* m_PropertyInformation;

        Visibility::Value m_Visibility;

    private:
        SizeF m_LastMeasureSize;
        SizeF m_DesiredSize;
        SizeF m_FinalSize;
};

template< >
struct ObjectTypeTraits< CUIElement >
{
    static const TypeIndex::Value Type = TypeIndex::UIElement;
};