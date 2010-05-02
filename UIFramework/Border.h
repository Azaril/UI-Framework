#pragma once

#include "Decorator.h"
#include "GeometryVisual.h"

class CBorder : public CDecorator
{
    public:
        DECLARE_FACTORY( CBorder );

        virtual TypeIndex::Value GetType() { return TypeIndex::Border; }
        virtual BOOL IsTypeOf( TypeIndex::Value Type ) { return Type == TypeIndex::Border || CDecorator::IsTypeOf(Type); }

        virtual HRESULT SetValue( CProperty* pProperty, CObjectWithType* pValue );

        virtual HRESULT SetChild( CUIElement* pChild );

        HRESULT SetBackground( CBrush* pBrush );

        HRESULT SetBorderThickness( FLOAT BorderThickness );
        HRESULT SetBorder( CBrush* pBrush );

        HRESULT SetPadding( const RectF& Padding );

        virtual HRESULT OnAttach( CUIAttachContext& Context );
        virtual HRESULT OnDetach( CUIDetachContext& Context );

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult );

    protected:
        CBorder();
        virtual ~CBorder();

        HRESULT Initialize();
        HRESULT Finalize();

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
        virtual HRESULT ArrangeInternal( SizeF Size );

        virtual HRESULT PreRenderInternal( CPreRenderContext& Context );

        virtual HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        HRESULT InternalSetPadding( const RectF& Padding );

        HRESULT RebuildGeometry();
        HRESULT ReleaseGeometry();

        FLOAT m_BorderThickness;
        CGeometryVisual* m_BorderVisual;
        BOOL m_GeometryDirty;

        RectF m_Padding;
};
