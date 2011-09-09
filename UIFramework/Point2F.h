#pragma once

#include "Defines.h"
#include "DataTypes.h"

#ifdef _WINDOWS
#include <D2D1.h>
#include <D2D1helper.h>
#endif

struct UIFRAMEWORK_API Point2F
#ifdef _WINDOWS
    : D2D1_POINT_2F
#endif
{
    Point2F(
        )
    {
        x = 0;
        y = 0;
    }

    Point2F(
        FLOAT X, 
        FLOAT Y
        )
    {
        x = X;
        y = Y;
    }
    
#ifndef _WINDOWS
    FLOAT x;
    FLOAT y;
#endif    
};

inline bool 
operator==(
    const Point2F& Val1,
    const Point2F& Val2 
    )
{
    return (Val1.x == Val2.x && Val1.y == Val2.y);
}