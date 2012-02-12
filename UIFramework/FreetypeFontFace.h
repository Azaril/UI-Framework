#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Factory.h"
#include "RefCounted.h"
#include "TextureAllocator.h"
#include "GlyphMetrics.h"
#include "FontMetrics.h"

class CFontDescription;
class CFreetypeTextFormat;
class CFreetypeFontSize;

class CFreetypeFontFace : public CRefCountedObject
{
    public:
        DECLARE_FACTORY1( CFreetypeFontFace, FT_Face );

        __checkReturn HRESULT GetGlyphMetrics(
            const UINT32 glyph,
            __in CFreetypeFontSize* pFontSize,
            __out GlyphMetrics* pGlyphMetrics
            );

        __checkReturn HRESULT LoadIntoTexture(
            const UINT32 glyph,
            __in CFreetypeFontSize* pFontSize,
            __in ITextureAllocator* pAllocator,
            __deref_out_opt ITexture** ppTexture
            );

        __checkReturn HRESULT GetFamilyNames(
            __deref_out_ecount(*pFamilyNameCount) const WCHAR*** pppFamilyNames,
            __out UINT32* pFamilyNameCount
            );

        __checkReturn HRESULT GetOrCreateFormat(
            __in const CFontDescription* pDescription,
            __in ITextureAllocator* pAllocator,
            __deref_out CFreetypeTextFormat** ppFormat
            );

        void RemoveCachedFormat(
            __in CFreetypeTextFormat* pFormat
            );

        __checkReturn HRESULT CreateFontSize(
            FLOAT fontSize,
            __deref_out CFreetypeFontSize** ppFontSize
            );

        __out FT_Face GetFontFace(
            );

        __checkReturn HRESULT GetSupportsKerning(
            __out bool* pSupportsKerning
            );

        __checkReturn HRESULT GetKerning(
            UINT32 leftGlyph,
            UINT32 rightGlyph,
            __in CFreetypeFontSize* pFontSize,
            __out Point2I* pKerning
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

        FT_Face m_pFontFace;
        bool m_LoadedFamilyNames;
        WCHAR** m_ppFamilyNames;
        UINT32 m_FamilyNameCount;
        list< CFreetypeTextFormat* > m_CachedFormats;
};

