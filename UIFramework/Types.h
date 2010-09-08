#pragma once

#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <algorithm>

#include "Defines.h"

#if _WINDOWS
#include <crtdbg.h>

#if defined(_DEBUG) && defined(TRACK_LEAKS)
#include "Signals.h"
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#else

inline FLOAT _wtof(const WCHAR* pStr)
{
    return wcstof(pStr, NULL);
}

#endif

#include "DataTypes.h"

#include "RectF.h"
#include "RectU.h"
#include "SizeU.h"
#include "SizeF.h"
#include "ColorF.h"
#include "Point2U.h"
#include "Point2F.h"
#include "Matrix3X2F.h"
#include "Gradient.h"

#ifndef ARRAYSIZE
#define ARRAYSIZE(a) (sizeof(a)/sizeof(a[0]))
#endif