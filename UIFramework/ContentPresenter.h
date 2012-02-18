#pragma once

#include "FrameworkElement.h"

class UIFRAMEWORK_API CContentPresenter : public CFrameworkElement
{
    public:
        DECLARE_FACTORY1( CContentPresenter, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::ContentPresenter, CFrameworkElement );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        HRESULT SetContent( CObjectWithType* pContent );

        virtual HRESULT OnAttach( CUIAttachContext& Context );
        virtual HRESULT OnDetach( CUIDetachContext& Context );

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult );

        //
        // Properties
        //
        static CStaticProperty ContentProperty;

    protected:
        CContentPresenter();
        virtual ~CContentPresenter();

        __override virtual __checkReturn HRESULT MeasureInternal( 
            const SizeF& AvailableSize,
            SizeF& DesiredSize
            );

        __override virtual __checkReturn HRESULT ArrangeInternal(
            const SizeF& AvailableSize,
            SizeF& UsedSize 
            );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        virtual CUIElement* GetTemplateParentForChildren();

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnContentChanged );

        virtual HRESULT OnContentChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        HRESULT GetUIForContent( CUIElement** ppContentUI );
        
        HRESULT RevokeContentUI();
        HRESULT EnsureContentUI();

        HRESULT GetContentChild( CUIElement** ppChild );

        CLayeredValue m_Content;
        bool m_ContentDirty;
        CUIElement* m_ContentChild;
};

template< >
struct ObjectTypeTraits< CContentPresenter >
{
    static const TypeIndex::Value Type = TypeIndex::ContentPresenter;
};