#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Factory.h"
#include "RefCounted.h"
#include "TextureAllocator.h"
#include "GlyphMetrics.h"

class CFreetypeFontFace : public CRefCountedObject
{
    public:
        DECLARE_FACTORY1( CFreetypeFontFace, FT_Face );

        __checkReturn HRESULT GetGlyphMetrics(
            const UINT32 glyph,
            const FLOAT& fontSize,
            __out GlyphMetrics* pGlyphMetrics
            );

        __checkReturn HRESULT LoadIntoTexture(
            const UINT32 glyph,
            const FLOAT& fontSize,
            __in ITextureAllocator* pAllocator,
            __deref_out_opt ITexture** ppTexture
            );

    protected:
        CFreetypeFontFace(
            );

        virtual ~CFreetypeFontFace(
            );
        
        __checkReturn HRESULT Initialize(
            __in FT_Face pFontFace
            );

        __checkReturn HRESULT LoadGlyph(
            UINT32 glyph
            );

        __checkReturn HRESULT SetFontSize(
            const FLOAT& fontSize
            );

        FT_Face m_pFontFace;
        FLOAT m_CurrentFontSize;
        UINT32 m_CurrentGlyph;
};

