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

        virtual HRESULT Arrange( RectF Bounds );

        HRESULT LineUp();
        HRESULT LineDown();

        HRESULT SetVerticalOffset( FLOAT Offset );
        HRESULT SetHorizontalOffset( FLOAT Offset );

        FLOAT GetVerticalOffset();
        FLOAT GetHorizontalOffset();

        SizeF GetExtent();
        SizeF GetViewport();

        HRESULT SetScrollOwner( IScrollOwner* pOwner );

    protected:
        CScrollContentPresenter();
        virtual ~CScrollContentPresenter();

        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT OnContentChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
        virtual HRESULT ArrangeInternal( SizeF AvailableSize, SizeF& UsedSize );

        virtual HRESULT PreRenderInternal( CPreRenderContext& Context );

        virtual BOOL ShouldClipToLayout();

        virtual const Matrix3X2F* GetChildRenderTransform();

        HRESULT DirtyScrollInformation();
        void DirtyChildTransform();

        HRESULT SetScrollOffsets( FLOAT XOffset, FLOAT YOffset );

        BOOL CanScrollHorizontally();
        BOOL CanScrollVertically();

        FLOAT m_VerticalOffset;
        FLOAT m_HorizontalOffset;

        Matrix3X2F m_ChildrenTransform;
        BOOL m_ChildTransformDirty;
        SizeF m_Extent;
        IScrollOwner* m_ScrollOwner;
};

template< >
struct ObjectTypeTraits< CScrollContentPresenter >
{
    static const TypeIndex::Value Type = TypeIndex::ScrollContentPresenter;
};