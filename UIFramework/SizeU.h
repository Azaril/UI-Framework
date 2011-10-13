#pragma once

#include "Defines.h"
#include "DataTypes.h"

struct UIFRAMEWORK_API SizeU
{
    SizeU(
        )
    {
        width = 0;
        height = 0;
    }
    
    SizeU(
        UINT32 Width,
        UINT32 Height
        )
    {
        width = Width;
        height = Height;
    }
    
    UINT32 width;
    UINT32 height;
};

inline bool 
    operator==(
    const SizeU& Val1,
    const SizeU& Val2
    )
{
    return (Val1.width == Val2.width && Val1.height == Val2.height);
}

inline bool 
    operator!=(
    const SizeU& Val1,
    const SizeU& Val2
    )
{
    return (Val1.width != Val2.width || Val1.height != Val2.height);
}