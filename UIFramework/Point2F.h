#pragma once

#include "Defines.h"
#include "DataTypes.h"
#include <math.h>

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
    
    inline Point2F operator*(
        FLOAT Scale
        ) const
    {
        return Point2F(x * Scale, y * Scale);
    }
    
    inline Point2F operator/(
        FLOAT Scale
        ) const
    {
        return Point2F(x / Scale, y / Scale);
    }    
    
    inline Point2F operator+(
        const Point2F& Other
        ) const
    {
        return Point2F(x + Other.x, y + Other.y);
    }
    
    inline Point2F operator-(
        const Point2F& Other
        ) const
    {
        return Point2F(x - Other.x, y - Other.y);
    }
    
    inline FLOAT MagnitudeSquared(
        ) const
    {
        return (x * x) + (y * y);
    }
    
    inline FLOAT Magnitude(
        ) const
    {
        return sqrt(MagnitudeSquared());
    }
    
    inline Point2F Normalize(
        ) const
    {
        return *this / Magnitude();
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

inline bool 
operator!=(
    const Point2F& Val1,
    const Point2F& Val2 
    )
{
    return (Val1.x != Val2.x || Val1.y != Val2.y);
}