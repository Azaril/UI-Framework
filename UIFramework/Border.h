#pragma once

#include "Decorator.h"
#include "GeometryVisual.h"

class CBorder : public CDecorator
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

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
        virtual HRESULT ArrangeInternal( SizeF AvailableSize, SizeF& UsedSize );

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

        HRESULT GetEffectiveBackground( CBrush** ppBrush );
        HRESULT GetEffectivePadding( RectF* pPadding );
        HRESULT GetEffectiveBorderThickness( FLOAT* pBorderThickness );
        HRESULT GetEffectiveBorderBrush( CBrush** ppBrush );
        HRESULT GetEffectiveCornerRadius( FLOAT* pCornerRadius );

        CTypedLayeredValue< CBrush > m_Background;
        CTypedLayeredValue< CRectFValue > m_Padding;
        CTypedLayeredValue< CFloatValue > m_BorderThickness;
        CTypedLayeredValue< CBrush > m_BorderBrush;
        CTypedLayeredValue< CFloatValue > m_CornerRadius;

        CGeometryVisual* m_BorderVisual;
        BOOL m_GeometryDirty;
};

template< >
struct ObjectTypeTraits< CBorder >
{
    static const TypeIndex::Value Type = TypeIndex::Border;
};