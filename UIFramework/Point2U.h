#pragma once

#include "Defines.h"
#include "DataTypes.h"

struct UIFRAMEWORK_API Point2U
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
    
    UINT32 x;
    UINT32 y;
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