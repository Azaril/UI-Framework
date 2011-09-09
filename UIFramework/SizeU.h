#pragma once

#include "Defines.h"
#include "DataTypes.h"

#ifdef _WINDOWS
#include <D2D1.h>
#include <D2D1helper.h>
#endif

struct UIFRAMEWORK_API SizeU
#ifdef _WINDOWS
    : D2D1_SIZE_U
#endif
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
    
#ifndef _WINDOWS
    UINT32 width;
    UINT32 height;
#endif    
};