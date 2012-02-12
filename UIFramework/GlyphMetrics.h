#pragma once

#include "Types.h"

struct GlyphMetrics
{
    Point2I HorizontalBearing;
    Point2I VerticalBearing;
    Point2U Size;
    UINT32 HorizontalAdvance;
    UINT32 VerticalAdvance;
};

