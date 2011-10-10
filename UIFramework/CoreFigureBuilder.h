#pragma once

#include "FigureBuilder.h"
#include "Factory.h"
#include "CoreFigureData.h"

class CCoreShapeData;

class CCoreFigureBuilder : public CFigureBuilder
{
    public:
        DECLARE_FACTORY1( CCoreFigureBuilder, CCoreShapeData* );

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
        CCoreFigureBuilder(
            );   

        virtual ~CCoreFigureBuilder(
            );

        __checkReturn HRESULT Initialize(
            __in CCoreShapeData* pShapeData
            );

        CCoreShapeData* m_pShapeData;
        vector< FigureDataType::Value > m_FigureDataType;
        vector< Point2F > m_FigurePoints;
};

