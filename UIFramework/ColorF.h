#pragma once

#include "Defines.h"
#include "DataTypes.h"
#include "Colors.h"

#ifdef _WINDOWS
#include <D2D1.h>
#include <D2D1helper.h>
#endif

struct UIFRAMEWORK_API ColorF
#ifdef _WINDOWS
    : D2D1_COLOR_F
#endif
{
    ColorF(
		)
    {
        r = 0;
        g = 0;
        b = 0;
        a = 0;
    }

#ifdef _WINDOWS
    explicit ColorF(
        const D2D1_COLOR_F& other
        )
    {
        r = other.r;
        g = other.g;
        b = other.b;
        a = other.a;        
    }
#endif

    ColorF(
		FLOAT Red, 
		FLOAT Green, 
		FLOAT Blue, 
		FLOAT Alpha
		)
    {
        r = Red;
        g = Green;
        b = Blue;
        a = Alpha;
    }

    ColorF(
		Color::Value Color, 
		FLOAT Alpha = 1.0f
		)
    {
        r = ((Color & (0xFF << 16)) >> 16) / 255.0f;
        g = ((Color & (0xFF << 8)) >> 8) / 255.0f;
        b = ((Color & (0xFF << 0)) >> 0) / 255.0f;
        a = Alpha;
    }
    
    static ColorF Interpolate(
        const ColorF& from,
        const ColorF& to,
        FLOAT val
        )
    {
        ColorF interoplatedColor(
            from.r + ((to.r - from.r) * val),
            from.g + ((to.g - from.g) * val),
            from.b + ((to.b - from.b) * val),
            from.a + ((to.a - from.a) * val)
        );
        
        return interoplatedColor;
    }
    
#ifndef _WINDOWS
    FLOAT r;
    FLOAT g;
    FLOAT b;
    FLOAT a;
#endif    
};

inline bool 
operator==(
	const ColorF& Val1, 
	const ColorF& Val2 
	)
{
    return (Val1.r == Val2.r && Val1.g == Val2.g && Val1.b == Val2.b && Val1.a == Val2.a);
}