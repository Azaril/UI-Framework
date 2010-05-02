#pragma once

#include "Visual.h"
#include "Geometry.h"
#include "Brush.h"

class CGeometryVisual : public CVisual
{
    public:
        DECLARE_FACTORY( CGeometryVisual );

        HRESULT SetGeometry( CGeometry* pGeometry );
        HRESULT SetFillBrush( CBrush* pBrush );
        HRESULT SetStrokeBrush( CBrush* pBrush );
        HRESULT SetStrokeThickness( FLOAT Thickness );

        virtual HRESULT PreRender( CPreRenderContext& Context );

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult );

    protected:
        CGeometryVisual();
        virtual ~CGeometryVisual();

        HRESULT Initialize();

        HRESULT InternalSetGeometry( CGeometry* pGeometry );
        HRESULT InternalSetFillBrush( CBrush* pBrush );
        HRESULT InternalSetStrokeBrush( CBrush* pBrush );
        HRESULT InternalSetStrokeThickness( FLOAT Thickness );

        virtual HRESULT RenderTransformed( CRenderContext& Context );

        CGeometry* m_Geometry;
        CBrush* m_FillBrush;
        CBrush* m_StrokeBrush;
        CGraphicsBrush* m_FillGraphicsBrush;
        CGraphicsBrush* m_StrokeGraphicsBrush;
        FLOAT m_StrokeThickness;
};
