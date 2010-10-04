#pragma once

#include "DataTypes.h"

#ifdef _WINDOWS
#include <D2D1.h>
#include <D2D1helper.h>
#endif

struct RectF
#ifdef _WINDOWS
    : D2D1_RECT_F
#endif
{
    RectF()
    {
        left = 0;
        top = 0;
        right = 0;
        bottom = 0;
    }
    
    RectF(FLOAT Left, FLOAT Top, FLOAT Right, FLOAT Bottom)
    {
        left = Left;
        top = Top;
        right = Right;
        bottom = Bottom;
    }

    inline BOOL IsInfinite()
    {
        return (left == -FLT_MAX && right == FLT_MAX && top == -FLT_MAX && bottom == FLT_MAX);
    }

    static inline RectF Infinite()
    {
        return RectF(-FLT_MAX, -FLT_MAX, FLT_MAX, FLT_MAX);
    }
    
#ifndef _WINDOWS
    FLOAT left;
    FLOAT top;
    FLOAT right;
    FLOAT bottom;
#endif    
};

inline bool operator==( const RectF& Val1, const RectF& Val2 )
{
    return (Val1.left == Val2.left && Val1.top == Val2.top && Val1.right == Val2.right && Val1.bottom == Val2.bottom);
}

inline bool operator!=( const RectF& Val1, const RectF& Val2 )
{
    return (Val1.left != Val2.left || Val1.top != Val2.top || Val1.right != Val2.right || Val1.bottom != Val2.bottom);
}

#include "SizeF.h"
#include "Point2F.h"

inline RectF MakeRect(Point2F Coords, SizeF Size)
{
    RectF Val(Coords.x, Coords.y, Coords.x + Size.width, Coords.y + Size.height);
    
    return Val;
}

inline RectF MakeRect(SizeF Coords, SizeF Size)
{
    RectF Val(Coords.width, Coords.height, Coords.width + Size.width, Coords.height + Size.height);
    
    return Val;
}

inline RectF MakeRect(SizeF Size)
{
    RectF Val(0, 0, Size.width, Size.height);
    
    return Val;
}