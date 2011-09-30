#pragma once

#include "Defines.h"
#include "DataTypes.h"
#include "Point2U.h"
#include "SizeU.h"

#ifdef _WINDOWS
#include <D2D1.h>
#include <D2D1helper.h>
#endif

struct UIFRAMEWORK_API RectU
#ifdef _WINDOWS
    : D2D1_RECT_U
#endif
{
    RectU(
        )
    {
        left = 0;
        top = 0;
        right = 0;
        bottom = 0;
    }

    RectU(
        UINT32 Left, 
        UINT32 Top,
        UINT32 Right,
        UINT32 Bottom
        )
    {
        left = Left;
        top = Top;
        right = Right;
        bottom = Bottom;
    }

    inline UINT32 GetWidth(
        ) const
    {
        return right - left;
    }

    inline UINT32 GetHeight(
        ) const
    {
        return bottom - top;
    }

    inline SizeU GetSize(
        ) const
    {
        return SizeU(GetWidth(), GetHeight());
    }

    inline Point2U GetTopLeft(
        ) const
    {
        return Point2U(top, left);
    }
    
#ifndef _WINDOWS
    UINT32 left;
    UINT32 top;
    UINT32 right;
    UINT32 bottom;
#endif    
};

inline bool 
    operator==(
    const RectU& Val1,
    const RectU& Val2
    )
{
    return (Val1.left == Val2.left && Val1.top == Val2.top && Val1.right == Val2.right && Val1.bottom == Val2.bottom);
}

inline bool 
    operator!=( 
    const RectU& Val1, 
    const RectU& Val2 
    )
{
    return (Val1.left != Val2.left || Val1.top != Val2.top || Val1.right != Val2.right || Val1.bottom != Val2.bottom);
}

inline RectU
    MakeRect(
    const Point2U& Coords,
    const SizeU& Size
    )
{
    RectU Val(Coords.x, Coords.y, Coords.x + Size.width, Coords.y + Size.height);

    return Val;
}

inline RectU 
    MakeRect(
    const SizeU& Coords, 
    const SizeU& Size
    )
{
    RectU Val(Coords.width, Coords.height, Coords.width + Size.width, Coords.height + Size.height);

    return Val;
}

inline RectU 
    MakeRect(
    const SizeU& Size
    )
{
    RectU Val(0, 0, Size.width, Size.height);

    return Val;
}