#pragma once

#include "Decorator.h"
#include "GeometryVisual.h"

class CBorder : public CDecorator
{
    public:
        DECLARE_FACTORY( CBorder );

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

        HRESULT Initialize();
        HRESULT Finalize();

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
        virtual HRESULT ArrangeInternal( SizeF Size );

        virtual HRESULT SetValueInternal( CProperty* pProperty, CObjectWithType* pValue );
        virtual HRESULT GetValueInternal( CProperty* pProperty, CObjectWithType** ppValue );

        virtual HRESULT PreRenderInternal( CPreRenderContext& Context );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        HRESULT SetPaddingInternal( const RectF& Padding );
        HRESULT SetCornerRadiusInternal( FLOAT Radius );

        HRESULT InvalidateGeometry();
        HRESULT RebuildGeometry();
        HRESULT ReleaseGeometry();

        static HRESULT StaticOnBackgroundChanged( CPropertyObject* pObjectInstance );
        HRESULT OnBackgroundChanged();

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