#pragma once

#if _WINDOWS

#include <stdlib.h>

#include <crtdbg.h>

#ifdef _DEBUG

#include "Signals.h"

#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

#endif

#include <windows.h>
#include <D2D1.h>
#include <D2D1helper.h>

typedef D2D1_SIZE_U SizeU;
typedef D2D1_SIZE_F SizeF;
typedef D2D1_COLOR_F ColorF;
typedef D2D1_RECT_F RectF;
typedef D2D1_POINT_2F Point2F;
typedef D2D_MATRIX_3X2_F Matrix3X2;
typedef D2D1_GRADIENT_STOP GradientStop;

inline bool operator==( const RectF& Val1, const RectF& Val2 )
{
    return (Val1.left == Val2.left && Val1.top == Val2.top && Val1.right == Val2.right && Val1.bottom == Val2.bottom);
}

inline bool operator==( const ColorF& Val1, const ColorF& Val2 )
{
    return (Val1.r == Val2.r && Val1.g == Val2.g && Val1.b == Val2.b && Val1.a == Val2.a);
}

inline bool operator==( const Point2F& Val1, const Point2F& Val2 )
{
    return (Val1.x == Val2.x && Val1.y == Val2.y);
}

#else

typedef int INT32;

typedef INT32 HRESULT;

#define NULL (0)

#define TRUE    1
#define FALSE   0

#endif

#include <float.h>

struct Nothing;