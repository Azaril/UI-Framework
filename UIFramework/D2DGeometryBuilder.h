#pragma once

#if defined(FRAMEWORK_D2D)

#include "GraphicsGeometryBuilder.h"
#include "Factory.h"

#include <d2d1.h>

class CD2DPathGeometry;

class CD2DGeometryBuilder : public CGraphicsGeometryBuilder
{
    public:
        DECLARE_FACTORY1( CD2DGeometryBuilder, ID2D1Factory* );

        __override virtual __checkReturn HRESULT GetGraphicsGeometry(
            __deref_out CGraphicsGeometry** ppGeometry
            );

        __override virtual __checkReturn HRESULT Close(
            );

        __override virtual __checkReturn HRESULT SetFillMode(
            FillMode::Value fillMode
            );

        __override virtual __checkReturn HRESULT BeginFigure(
            const Point2F& startPoint,
            BeginFigureOptions::Value options
            );

        __override virtual __checkReturn HRESULT EndFigure(
            EndFigureOptions::Value options
            );

        __override virtual __checkReturn HRESULT AddLines(
            __in_ecount(PointCount) const Point2F* pPoints,
            UINT32 Points
            );

        __override virtual __checkReturn HRESULT AddBeziers(
            __in_ecount(BezierCount) const BezierSegment* pBeziers,
            UINT32 BezierCount
            );

    protected:
        CD2DGeometryBuilder(
            );

        virtual ~CD2DGeometryBuilder(
            );

        __checkReturn HRESULT Initialize(
            __in ID2D1Factory* pFactory
            );

        CD2DPathGeometry* m_pPathGeometry;
        ID2D1GeometrySink* m_pGeometrySink;
};

#endif