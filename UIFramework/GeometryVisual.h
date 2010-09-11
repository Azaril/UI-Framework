#pragma once

#include "Visual.h"
#include "Geometry.h"
#include "Brush.h"

class UIFRAMEWORK_API CGeometryVisual : public CVisual
{
    public:
        DECLARE_FACTORY( CGeometryVisual );

        DECLARE_TYPE_WITH_BASE( TypeIndex::GeometryVisual, CVisual );

        HRESULT SetGeometry( CGeometry* pGeometry );
        HRESULT SetFillBrush( CBrush* pBrush );
        HRESULT SetStrokeBrush( CBrush* pBrush );
        HRESULT SetStrokeThickness( FLOAT Thickness );

        HRESULT SetFillBrushTransform( const Matrix3X2F& Transform );

        virtual HRESULT PreRender( CPreRenderContext& Context );

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult );

        virtual HRESULT OnVisualNotification( CVisualNotification* pNotification );

    protected:
        CGeometryVisual();
        virtual ~CGeometryVisual();

        HRESULT Initialize();

        HRESULT InternalSetGeometry( CGeometry* pGeometry );
        HRESULT InternalSetFillBrush( CBrush* pBrush );
        HRESULT InternalSetStrokeBrush( CBrush* pBrush );
        HRESULT InternalSetStrokeThickness( FLOAT Thickness );

        HRESULT OnBrushInvalidated( CBrush* pBrush );

        virtual HRESULT RenderTransformed( CRenderContext& Context );

        CGeometry* m_Geometry;
        CBrush* m_FillBrush;
        CBrush* m_StrokeBrush;
        CGraphicsBrush* m_FillGraphicsBrush;
        CGraphicsBrush* m_StrokeGraphicsBrush;
        FLOAT m_StrokeThickness;
        Matrix3X2F m_FillBrushTransform;
};
