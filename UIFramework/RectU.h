#pragma once

#include "DataTypes.h"

#ifdef _WINDOWS
#include <D2D1.h>
#include <D2D1helper.h>
#endif

struct RectU
#ifdef _WINDOWS
    : D2D1_RECT_U
#endif
{
    RectU()
    {
        left = 0;
        top = 0;
        right = 0;
        bottom = 0;
    }

    RectU(UINT32 Left, UINT32 Top, UINT32 Right, UINT32 Bottom)
    {
        left = Left;
        top = Top;
        right = Right;
        bottom = Bottom;
    }
    
#ifndef _WINDOWS
    UINT32 left;
    UINT32 top;
    UINT32 right;
    UINT32 bottom;
#endif    
};