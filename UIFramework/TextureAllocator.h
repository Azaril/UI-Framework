#pragma once

#include "Texture.h"

struct ITextureAllocator
{
    virtual __checkReturn HRESULT AllocateTexture(
        UINT32 Width,
        UINT32 Height,
        __deref_out ITexture** ppTexture
        ) = 0;
};

struct IBatchUpdateTextureAllocator : public ITextureAllocator
{
    virtual __checkReturn HRESULT AllocateTexture(
        UINT32 Width,
        UINT32 Height,
        __deref_out IBatchUpdateTexture** ppTexture
        ) = 0;
};