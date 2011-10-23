#pragma once

#include "Types.h"
#include "Texture.h"

namespace ColorUtilities
{
    __checkReturn HRESULT FillTextureWithColor(
        __in ITexture* pTexture,
        ColorF& color
        );

    __checkReturn HRESULT FillTextureWithColor(
        __in IBatchUpdateTexture* pTexture,
        __in_ecount(RegionCount) const RectU* pRegions,
        UINT32 RegionCount,
        ColorF& color
        );
}