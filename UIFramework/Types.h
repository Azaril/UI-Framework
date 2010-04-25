#pragma once

#include <float.h>

#if _WINDOWS

#include <windows.h>
#include <D2D1.h>
#include <D2D1helper.h>

typedef D2D1_SIZE_U SizeU;
typedef D2D1_SIZE_F SizeF;
typedef D2D1_COLOR_F ColorF;
typedef D2D1_RECT_F RectF;
typedef D2D1_POINT_2F Point2F;
typedef D2D_MATRIX_3X2_F Matrix3X2;

#else

typedef int INT32;

typedef INT32 HRESULT;

#define NULL (0)

#define TRUE    1
#define FALSE   0

#endif

inline bool IsPositiveInfinity(double Val)
{
    return _fpclass(Val) == _FPCLASS_PINF;
}