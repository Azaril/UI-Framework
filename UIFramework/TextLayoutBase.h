#pragma once

#include "TextLayout.h"
#include "Texture.h"
#include "GlyphRun.h"

struct ITextLayoutRenderCallback
{
    virtual __checkReturn HRESULT RenderGlyphRun(
        __in ITexture* pTexture,
        __in GlyphRun* pGlyphRun,
        __in_opt void* pContext
        ) = 0;
};

class CTextLayoutBase : public CTextLayout
{
    public:
        virtual __checkReturn HRESULT Render(
            __in ITextLayoutRenderCallback* pCallback,
            __in_opt void* pContext
            ) = 0;
};

