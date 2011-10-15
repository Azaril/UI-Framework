#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Factory.h"
#include "RefCounted.h"
#include "TextureAllocator.h"

class CFreetypeFontFace : public CRefCountedObject
{
    public:
        DECLARE_FACTORY1( CFreetypeFontFace, FT_Face );

        __checkReturn HRESULT SetFontSize(
            const FLOAT& fontSize
            );

        __checkReturn HRESULT LoadGlyph(
            const WCHAR glyph
            );

        __checkReturn HRESULT LoadIntoTexture(
            __in ITextureAllocator* pAllocator,
            __deref_out ITexture** ppTexture
            );

    protected:
        CFreetypeFontFace(
            );

        virtual ~CFreetypeFontFace(
            );
        
        __checkReturn HRESULT Initialize(
            __in FT_Face pFontFace
            );

        FT_Face m_pFontFace;
};

