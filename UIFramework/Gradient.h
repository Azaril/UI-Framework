#pragma once

#include "Defines.h"
#include "DataTypes.h"
#include "ColorF.h"

struct UIFRAMEWORK_API GradientStop
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
    
    inline bool operator<(
        const GradientStop& Other
        ) const
    {
        return position < Other.position;
    }
    
    FLOAT position;
    ColorF color;
};