#pragma once

#include "Texture.h"

struct IStagingTextureCallback
{
    virtual __checkReturn HRESULT AddUpdate(
        __in ITexture* pSource,
        const RectU& sourceRect,
        __in ITexture* pDestination,
        const Point2U& destOffset
        ) = 0;
};