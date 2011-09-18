#pragma once

#include "Types.h"

struct RenderVertex
{
    Point2F Position;
    ColorF Color;
};

#define RENDERVERTEX_POSITION_OFFSET    (0)
#define RENDERVERTEX_COLOR_OFFSET       (RENDERVERTEX_POSITION_OFFSET + (2 * sizeof(FLOAT)))