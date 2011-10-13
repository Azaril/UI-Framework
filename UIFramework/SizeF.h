#pragma once

#include "Defines.h"
#include "DataTypes.h"

struct UIFRAMEWORK_API SizeF
{
    SizeF(
        )
    {
        width = 0;
        height = 0;
    }

    SizeF(
        FLOAT Width,
        FLOAT Height
        )
    {
        width = Width;
        height = Height;
    }
    
    FLOAT width;
    FLOAT height;
};

inline bool 
operator==(
    const SizeF& Val1,
    const SizeF& Val2 
    )
{
    return (Val1.width == Val2.width && Val1.height == Val2.height);
}

inline bool 
operator!=(
    const SizeF& Val1, 
    const SizeF& Val2 
    )
{
    return (Val1.width != Val2.width || Val1.height != Val2.height);
}