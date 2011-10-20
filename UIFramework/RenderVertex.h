#pragma once

#include "Types.h"

struct RenderVertex
{
    Point2F Position;
    ColorF Color;
    Point2F TextureCoordinates;
    Point2F MaskCoordinates;
};

#define RENDERVERTEX_POSITION_OFFSET    (0)
#define RENDERVERTEX_COLOR_OFFSET       (RENDERVERTEX_POSITION_OFFSET + (2 * sizeof(FLOAT)))
#define RENDERVERTEX_TEXCOORDS_OFFSET   (RENDERVERTEX_COLOR_OFFSET + (4 * sizeof(FLOAT)))
#define RENDERVERTEX_MASKCOORDS_OFFSET  (RENDERVERTEX_TEXCOORDS_OFFSET + (2 * sizeof(FLOAT)))