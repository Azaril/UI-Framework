#pragma once

#include "Defines.h"
#include "DataTypes.h"

struct UIFRAMEWORK_API Point2I
{
    Point2I(
        )
    {
        x = 0;
        y = 0;
    }

    Point2I(
        INT32 X, 
        INT32 Y
        )
    {
        x = X;
        y = Y;
    }

    inline Point2I& operator+=(
        const Point2I& other
        )
    {
        x += other.x;
        y += other.y;

        return (*this);
    }

    inline Point2I& operator-=(
        const Point2I& other
        )
    {
        x -= other.x;
        y -= other.y;

        return (*this);
    }
    
    INT32 x;
    INT32 y;
};

inline bool 
operator==(
    const Point2I& Val1,
    const Point2I& Val2 
    )
{
    return (Val1.x == Val2.x && Val1.y == Val2.y);
}

inline bool 
    operator!=(
    const Point2I& Val1,
    const Point2I& Val2 
    )
{
    return (Val1.x != Val2.x || Val1.y != Val2.y);
}

inline Point2I 
    operator+(
    const Point2I& Val1,
    const Point2I& Val2 
    )
{
    return Point2I(Val1.x + Val2.x, Val1.y + Val2.y);
}

inline Point2I 
    operator-(
    const Point2I& Val1,
    const Point2I& Val2 
    )
{
    return Point2I(Val1.x - Val2.x, Val1.y - Val2.y);
}