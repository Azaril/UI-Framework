#pragma once

#include "Visual.h"
#include "GraphicsGeometry.h"
#include "Brush.h"

class UIFRAMEWORK_API CGeometryVisual : public CVisual
{
    public:
        DECLARE_FACTORY( CGeometryVisual );

        DECLARE_TYPE_WITH_BASE( TypeIndex::GeometryVisual, CVisual );

        __checkReturn HRESULT SetGeometry(
            __in_opt CGraphicsGeometry* pGeometry 
            );

        __checkReturn HRESULT SetFillBrush(
            __in_opt CBrush* pBrush 
            );

        __checkReturn HRESULT SetStrokeBrush( 
            __in_opt CBrush* pBrush 
            );

        __checkReturn HRESULT SetStrokeThickness(
            FLOAT Thickness 
            );

        __checkReturn HRESULT SetBrushTransform( 
            const Matrix3X2F& Transform 
            );

        __checkReturn virtual HRESULT PreRender(
            CPreRenderContext& Context 
            );

        __checkReturn virtual HRESULT HitTest(
            Point2F LocalPoint, 
            __deref_out CHitTestResult** ppHitTestResult 
            );

    protected:
        CGeometryVisual(
            );

        virtual ~CGeometryVisual(
            );

        __checkReturn HRESULT Initialize(
            );

        __checkReturn HRESULT InternalSetGeometry( 
            __in_opt CGraphicsGeometry* pGeometry 
            );

        __checkReturn HRESULT InternalSetFillBrush(
            __in_opt CBrush* pBrush 
            );

        __checkReturn HRESULT InternalSetStrokeBrush( 
            __in_opt CBrush* pBrush 
            );

        __checkReturn HRESULT InternalSetStrokeThickness(
            FLOAT Thickness 
            );

        virtual HRESULT RenderTransformed( CRenderContext& Context );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnFillBrushChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnStrokeBrushChanged );

        __checkReturn HRESULT OnFillBrushChanged(
            __in_opt CObjectWithType* pOldValue, 
            __in_opt CObjectWithType* pNewValue 
            );

        __checkReturn HRESULT OnStrokeBrushChanged(
            __in_opt CObjectWithType* pOldValue, 
            __in_opt CObjectWithType* pNewValue 
            );

        CGraphicsGeometry* m_Geometry;
        CBrush* m_FillBrush;
        CBrush* m_StrokeBrush;
        CGraphicsBrush* m_FillGraphicsBrush;
        CGraphicsBrush* m_StrokeGraphicsBrush;
        FLOAT m_StrokeThickness;
        Matrix3X2F m_FillBrushTransform;
        Matrix3X2F m_StrokeBrushTransform;
        Matrix3X2F m_BrushTransform;
        BOOL m_UpdateFillTransform;
        BOOL m_UpdateStrokeTransform;
};

template< >
struct ObjectTypeTraits< CGeometryVisual >
{
    static const TypeIndex::Value Type = TypeIndex::GeometryVisual;
};