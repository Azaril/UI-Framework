#pragma once

#include "Types.h"
#include "RefCounted.h"

struct ITesselationSink
{
    virtual __checkReturn HRESULT AddTriangle(
        const Point2F& point1,
        const Point2F& point2,
        const Point2F& point3
        ) = 0;
};