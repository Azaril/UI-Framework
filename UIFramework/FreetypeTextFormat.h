#pragma once

#include "TextFormat.h"
#include "Factory.h"
#include "RefCounted.h"
#include "TextureAllocator.h"
#include "FreetypeFontFace.h"
#include "GlyphMetrics.h"
#include "FreetypeFontSize.h"

class CFreetypeTextFormat : public CTextFormat
{
    public:
        DECLARE_FACTORY3( CFreetypeTextFormat, CFreetypeFontFace*, FLOAT, ITextureAllocator* );

        __checkReturn HRESULT GetFontMetrics(
            __deref_out const FontMetrics** ppFontMetrics
            );

        __checkReturn HRESULT GetGlyphMetics(
            const UINT32 glyph,
            __deref_out const GlyphMetrics** ppGlyphMetrics
            );

        __checkReturn HRESULT GetGlyphTexture(
            const UINT32 glyph,
            __deref_out_opt ITexture** ppTexture
            );

        FLOAT GetFontSize(
            );

        __checkReturn HRESULT GetSupportsKerning(
            __out bool* pSupportsKerning
            );

        __checkReturn HRESULT GetKerning(
            UINT32 leftGlyph,
            UINT32 rightGlyph,
            __out Point2I* pKerning
            );

    protected:
        CFreetypeTextFormat(
            );

        virtual ~CFreetypeTextFormat(
            );

        __checkReturn HRESULT Initialize(
            __in CFreetypeFontFace* pFontFace,
            FLOAT fontSize,
            __in ITextureAllocator* pTextureAllocator
            );

        CFreetypeFontFace* m_pFontFace;
        CFreetypeFontSize* m_pFontSize;
        FLOAT m_fontSize;
        ITextureAllocator* m_pTextureAllocator;
        map< UINT32, ITexture* > m_GlyphTextures;
        map< UINT32, GlyphMetrics* > m_GlyphMetrics;
        bool m_HaveFontMetrics;
        FontMetrics m_FontMetrics;
};

