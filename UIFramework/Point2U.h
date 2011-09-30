#pragma once

#include "Defines.h"
#include "DataTypes.h"

#ifdef _WINDOWS
#include <D2D1.h>
#include <D2D1helper.h>
#endif

struct UIFRAMEWORK_API Point2U
#ifdef _WINDOWS
    : D2D1_POINT_2U
#endif
{
    Point2U(
        )
    {
        x = 0;
        y = 0;
    }

    Point2U(
        UINT32 X, 
        UINT32 Y
        )
    {
        x = X;
        y = Y;
    }
    
#ifndef _WINDOWS
    UINT32 x;
    UINT32 y;
#endif    
};

inline bool 
operator==(
    const Point2U& Val1,
    const Point2U& Val2 
    )
{
    return (Val1.x == Val2.x && Val1.y == Val2.y);
}

inline bool 
    operator!=(
    const Point2U& Val1,
    const Point2U& Val2 
    )
{
    return (Val1.x != Val2.x || Val1.y != Val2.y);
}

inline Point2U 
    operator+(
    const Point2U& Val1,
    const Point2U& Val2 
    )
{
    return Point2U(Val1.x + Val2.x, Val1.y + Val2.y);
}