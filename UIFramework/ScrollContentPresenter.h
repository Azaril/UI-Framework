#pragma once

#include "ContentPresenter.h"

struct IScrollOwner
{
    virtual HRESULT InvalidateScrollInformation() = 0;
};

class UIFRAMEWORK_API CScrollContentPresenter : public CContentPresenter
{
    public:
        DECLARE_FACTORY1( CScrollContentPresenter, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::ScrollContentPresenter, CContentPresenter );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT Arrange( RectF Bounds );

        HRESULT SetCanScrollVertically( bool CanScroll );
        HRESULT SetCanScrollHorizontally( bool CanScroll );

        HRESULT LineUp();
        HRESULT LineDown();

        HRESULT SetVerticalOffset( FLOAT Offset );
        HRESULT SetHorizontalOffset( FLOAT Offset );

        FLOAT GetVerticalOffset();
        FLOAT GetHorizontalOffset();

        SizeF GetExtent();
        SizeF GetViewport();

        HRESULT SetScrollOwner( IScrollOwner* pOwner );

        //
        // Properties
        //
        static CStaticProperty CanScrollVerticallyProperty;
        static CStaticProperty CanScrollHorizontallyProperty;

    protected:
        CScrollContentPresenter();
        virtual ~CScrollContentPresenter();

        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        virtual HRESULT OnContentChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
        virtual HRESULT ArrangeInternal( SizeF AvailableSize, SizeF& UsedSize );

        virtual HRESULT PreRenderInternal( CPreRenderContext& Context );

        virtual bool ShouldClipToLayout();

        virtual const Matrix3X2F* GetChildRenderTransform();

        HRESULT DirtyScrollInformation();
        void DirtyChildTransform();

        HRESULT SetScrollOffsets( FLOAT XOffset, FLOAT YOffset );

        HRESULT GetEffectiveCanScrollVertically( bool* pCanScroll );
        HRESULT GetEffectiveCanScrollHorizontally( bool* pCanScroll );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnCanScrollVerticallyChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnCanScrollHorizontallyChanged );

        HRESULT OnCanScrollVerticallyChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnCanScrollHorizontallyChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        FLOAT m_VerticalOffset;
        FLOAT m_HorizontalOffset;

        Matrix3X2F m_ChildrenTransform;
        bool m_ChildTransformDirty;
        SizeF m_Extent;
        SizeF m_Viewport;
        IScrollOwner* m_ScrollOwner;

        CTypedLayeredValue< CScrollContentPresenter, CBoolValue > m_CanScrollVertically;
        CTypedLayeredValue< CScrollContentPresenter, CBoolValue > m_CanScrollHorizontally;
};

template< >
struct ObjectTypeTraits< CScrollContentPresenter >
{
    static const TypeIndex::Value Type = TypeIndex::ScrollContentPresenter;
};