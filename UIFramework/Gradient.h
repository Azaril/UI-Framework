#pragma once

#include "Defines.h"
#include "DataTypes.h"
#include "ColorF.h"

#ifdef _WINDOWS
#include <D2D1.h>
#include <D2D1helper.h>
#endif

struct UIFRAMEWORK_API GradientStop
#ifdef _WINDOWS
    : D2D1_GRADIENT_STOP
#endif
{
    GradientStop(
		)
    {
        position = 0;
    }

    GradientStop(
		FLOAT Position, 
		ColorF Color
		)
    {
        position = Position;
        color = Color;
    }
    
#ifndef _WINDOWS
    FLOAT position;
    ColorF color;
#endif    
};