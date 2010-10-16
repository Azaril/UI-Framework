#pragma once

#include "DataTypes.h"

#ifdef _WINDOWS
#include <D2D1.h>
#include <D2D1helper.h>
#endif

struct SizeF
#ifdef _WINDOWS
    : D2D1_SIZE_F
#endif
{
    SizeF()
    {
        width = 0;
        height = 0;
    }

    SizeF(FLOAT Width, FLOAT Height)
    {
        width = Width;
        height = Height;
    }
    
#ifndef _WINDOWS
    FLOAT width;
    FLOAT height;
#endif    
};

inline bool operator==( const SizeF& Val1, const SizeF& Val2 )
{
    return (Val1.width == Val2.width && Val1.height == Val2.height);
}

inline bool operator!=( const SizeF& Val1, const SizeF& Val2 )
{
    return (Val1.width != Val2.width || Val1.height != Val2.height);
}