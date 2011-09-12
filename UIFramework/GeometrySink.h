#pragma once

#include "Types.h"

namespace BeginFigureOptions
{
    enum Value
    {
        Filled  = 0x00,
        Hollow  = 0x01
    };
}

namespace EndFigureOptions
{
    enum Value
    {
        Open    = 0x00,
        Closed  = 0x01
    };
}

namespace FillMode
{
    enum Value
    {
        Alternate   = 0x00,
        Winding     = 0x01
    };
}

struct UIFRAMEWORK_API IGeometrySink
{
    virtual __checkReturn HRESULT Close(
        ) = 0;

    virtual __checkReturn HRESULT SetFillMode(
        FillMode::Value fillMode
        ) = 0;

    virtual __checkReturn HRESULT BeginFigure(
        const Point2F& startPoint,
        BeginFigureOptions::Value options
        ) = 0;

    virtual __checkReturn HRESULT EndFigure(
        EndFigureOptions::Value options
        ) = 0;

    virtual __checkReturn HRESULT AddLines(
        __in_ecount(PointCount) const Point2F* pPoints,
        UINT32 Points
        ) = 0;

    virtual __checkReturn HRESULT AddBeziers(
        __in_ecount(BezierCount) const BezierSegment* pBeziers,
        UINT32 BezierCount
        ) = 0;
};

