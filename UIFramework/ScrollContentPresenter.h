#pragma once

#include "ContentPresenter.h"

class CScrollContentPresenter : public CContentPresenter
{
    public:
        DECLARE_FACTORY1( CScrollContentPresenter, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::ScrollContentPresenter, CContentPresenter );

        HRESULT LineUp();
        HRESULT LineDown();

        FLOAT GetVerticalOffset();
        FLOAT GetHorizontalOffset();

    protected:
        CScrollContentPresenter();
        virtual ~CScrollContentPresenter();

        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT OnContentChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
        virtual HRESULT ArrangeInternal( SizeF AvailableSize, SizeF& UsedSize );

        BOOL CanScrollHorizontally();
        BOOL CanScrollVertically();

        FLOAT m_VerticalOffset;
        FLOAT m_HorizontalOffset;
};

template< >
struct ObjectTypeTraits< CScrollContentPresenter >
{
    static const TypeIndex::Value Type = TypeIndex::ScrollContentPresenter;
};