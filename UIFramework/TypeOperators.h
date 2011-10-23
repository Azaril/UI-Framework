#pragma once

#include "Point2U.h"
#include "Point2I.h"

inline Point2I 
operator+(
    const Point2I& Val1,
    const Point2U& Val2 
    )
{
    return Point2I(Val1.x + Val2.x, Val1.y + Val2.y);
}

inline Point2I 
operator-(
    const Point2I& Val1,
    const Point2U& Val2 
    )
{
    return Point2I(Val1.x - Val2.x, Val1.y - Val2.y);
}

inline Point2I 
operator+(
    const Point2U& Val1,
    const Point2I& Val2 
    )
{
    return Point2I(Val1.x + Val2.x, Val1.y + Val2.y);
}

inline Point2I 
operator-(
    const Point2U& Val1,
    const Point2I& Val2 
    )
{
    return Point2I(Val1.x - Val2.x, Val1.y - Val2.y);
}

