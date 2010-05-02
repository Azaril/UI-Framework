#pragma once

#include "Visual.h"
#include "Geometry.h"
#include "Brush.h"

class CGeometryVisual : public CVisual
{
    public:
        DECLARE_FACTORY( CGeometryVisual );

        HRESULT SetGeometry( CGeometry* pGeometry );
        HRESULT SetBrush( CBrush* pBrush );

        virtual HRESULT PreRender( CPreRenderContext& Context );

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult );

    protected:
        CGeometryVisual();
        virtual ~CGeometryVisual();

        HRESULT Initialize();

        HRESULT InternalSetGeometry( CGeometry* pGeometry );
        HRESULT InternalSetBrush( CBrush* pBrush );

        virtual HRESULT RenderTransformed( CRenderContext& Context );

        CGeometry* m_Geometry;
        CBrush* m_Brush;
        CGraphicsBrush* m_GraphicsBrush;
};
