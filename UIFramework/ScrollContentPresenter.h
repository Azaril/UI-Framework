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

        __override virtual __checkReturn HRESULT Arrange(
            const RectF& Bounds 
            );

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

        __override virtual __checkReturn HRESULT MeasureInternal( 
            const SizeF& AvailableSize,
            SizeF& DesiredSize
            );

        __override virtual __checkReturn HRESULT ArrangeInternal(
            const SizeF& AvailableSize,
            SizeF& UsedSize 
            );

        virtual HRESULT PreRenderInternal( CPreRenderContext& Context );

        virtual bool ShouldClipToLayout();

        virtual const Matrix3X2F* GetChildRenderTransform();

        HRESULT DirtyScrollInformation();
        void DirtyChildTransform();

        HRESULT SetScrollOffsets( FLOAT XOffset, FLOAT YOffset );

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

        CLayeredValue m_CanScrollVertically;
        CLayeredValue m_CanScrollHorizontally;
};

template< >
struct ObjectTypeTraits< CScrollContentPresenter >
{
    static const TypeIndex::Value Type = TypeIndex::ScrollContentPresenter;
};