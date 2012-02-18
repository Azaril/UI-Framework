#pragma once

#include "Decorator.h"
#include "GeometryVisual.h"

class UIFRAMEWORK_API CBorder : public CDecorator
{
    public:
        DECLARE_FACTORY1( CBorder, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::Border, CDecorator );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT SetChild( CUIElement* pChild );

        HRESULT SetBackground( CBrush* pBrush );

        HRESULT SetBorderThickness( FLOAT BorderThickness );
        HRESULT SetBorder( CBrush* pBrush );

        HRESULT SetPadding( const RectF& Padding );

        HRESULT SetCornerRadius( FLOAT Radius );

        virtual HRESULT OnAttach( CUIAttachContext& Context );
        virtual HRESULT OnDetach( CUIDetachContext& Context );

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult );

        //
        // Properties
        //
        static CStaticProperty BackgroundProperty;
        static CStaticProperty PaddingProperty;
        static CStaticProperty BorderThicknessProperty;
        static CStaticProperty BorderBrushProperty;
        static CStaticProperty CornerRadiusProperty;

    protected:
        CBorder();
        virtual ~CBorder();

        HRESULT Initialize( CProviders* pProviders );
        HRESULT Finalize();

        __override virtual __checkReturn HRESULT MeasureInternal( 
            const SizeF& AvailableSize,
            SizeF& DesiredSize
            );

        __override virtual __checkReturn HRESULT ArrangeInternal(
            const SizeF& AvailableSize,
            SizeF& UsedSize 
            );

        virtual HRESULT PreRenderInternal( CPreRenderContext& Context );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        HRESULT SetPaddingInternal( const RectF& Padding );
        HRESULT SetCornerRadiusInternal( FLOAT Radius );

        HRESULT InvalidateGeometry();
        HRESULT RebuildGeometry();
        HRESULT ReleaseGeometry();

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnBackgroundChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnPaddingChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnBorderThicknessChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnBorderBrushChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnCornerRadiusChanged );

        HRESULT OnBackgroundChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnPaddingChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnBorderThicknessChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnBorderBrushChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnCornerRadiusChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        CLayeredValue m_Background;
        CLayeredValue m_Padding;
        CLayeredValue m_BorderThickness;
        CLayeredValue m_BorderBrush;
        CLayeredValue m_CornerRadius;

        CGeometryVisual* m_BorderVisual;
        bool m_GeometryDirty;
};

template< >
struct ObjectTypeTraits< CBorder >
{
    static const TypeIndex::Value Type = TypeIndex::Border;
};