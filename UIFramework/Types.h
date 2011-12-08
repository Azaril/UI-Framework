#pragma once

#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <algorithm>
#include <ostream>
#include <istream>

#include "Collections.h"

#if WIN32
#include <crtdbg.h>

#if defined(FRAMEWORK_DEBUG) && defined(TRACK_LEAKS)
#include "Signals.h"
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#else

inline float 
_wtof(
    const wchar_t* pStr
    )
{
    return wcstof(pStr, NULL);
}

#define _wcsicmp wcscasecmp

#define _isnan(val) std::isnan(val)

#endif

#include "Defines.h"

#include "DataTypes.h"

#include "RectF.h"
#include "RectU.h"
#include "SizeU.h"
#include "SizeF.h"
#include "ColorF.h"
#include "Point2I.h"
#include "Point2U.h"
#include "Point2F.h"
#include "Matrix3X2F.h"
#include "Gradient.h"
#include "BezierSegment.h"
#include "TypeOperators.h"

#ifndef ARRAYSIZE
#define ARRAYSIZE(a) (sizeof(a)/sizeof(a[0]))
#endif