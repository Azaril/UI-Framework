#pragma once

#include "Defines.h"
#include "DataTypes.h"
#include "Point2F.h"
#include "SizeF.h"

#pragma push_macro("min")
#pragma push_macro("max")

#undef min
#undef max

struct UIFRAMEWORK_API RectF
{
    RectF(
        )
    {
        left = 0;
        top = 0;
        right = 0;
        bottom = 0;
    }
    
    RectF(
        FLOAT Left,
        FLOAT Top,
        FLOAT Right,
        FLOAT Bottom
        )
    {
        left = Left;
        top = Top;
        right = Right;
        bottom = Bottom;
    }

    inline bool IsInfinite(
        )
    {
        return (left == std::numeric_limits< FLOAT >::min() && right == std::numeric_limits< FLOAT >::max() && top == std::numeric_limits< FLOAT >::min() && bottom == std::numeric_limits< FLOAT >::max());
    }

    static inline RectF Infinite(
        )
    {
        return RectF(std::numeric_limits< FLOAT >::min(), std::numeric_limits< FLOAT >::min(), std::numeric_limits< FLOAT >::max(), std::numeric_limits< FLOAT >::max());
    }
    
    inline bool ContainsPoint(
        const Point2F& Point
        )
    {
        return (Point.x >= left && Point.x <= right && Point.y >= top && Point.y <= bottom);
    }
    
    inline FLOAT GetWidth(
        ) const
    {
        return right - left;
    }
    
    inline FLOAT GetHeight(
    ) const
    {
        return bottom - top;
    }
    
    inline SizeF GetSize(
        ) const
    {
        return SizeF(GetWidth(), GetHeight());
    }
    
    inline Point2F GetTopLeft(
        ) const
    {
        return Point2F(left, top);
    }    
    
    FLOAT left;
    FLOAT top;
    FLOAT right;
    FLOAT bottom;
};

inline bool 
operator==(
    const RectF& Val1,
    const RectF& Val2
    )
{
    return (Val1.left == Val2.left && Val1.top == Val2.top && Val1.right == Val2.right && Val1.bottom == Val2.bottom);
}

inline bool 
operator!=( 
    const RectF& Val1, 
    const RectF& Val2 
    )
{
    return (Val1.left != Val2.left || Val1.top != Val2.top || Val1.right != Val2.right || Val1.bottom != Val2.bottom);
}

inline RectF 
MakeRect(
    const Point2F& Coords,
    const SizeF& Size
    )
{
    RectF Val(Coords.x, Coords.y, Coords.x + Size.width, Coords.y + Size.height);
    
    return Val;
}

inline RectF 
MakeRect(
    const SizeF& Coords, 
    const SizeF& Size
    )
{
    RectF Val(Coords.width, Coords.height, Coords.width + Size.width, Coords.height + Size.height);
    
    return Val;
}

inline RectF 
MakeRect(
    const SizeF& Size
    )
{
    RectF Val(0, 0, Size.width, Size.height);
    
    return Val;
}

#pragma pop_macro("min")
#pragma pop_macro("max")