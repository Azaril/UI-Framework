#pragma once

#include "TextFormat.h"
#include "Factory.h"
#include "RefCounted.h"
#include "TextureAllocator.h"
#include "FreetypeFontFace.h"

class CFreetypeTextFormat : public CTextFormat
{
    public:
        DECLARE_FACTORY3( CFreetypeTextFormat, CFreetypeFontFace*, FLOAT, ITextureAllocator* );

        __checkReturn HRESULT BeginQuery(
            );

        __checkReturn HRESULT EndQuery(
            );

        __checkReturn HRESULT LoadGlyph(
            const WCHAR glyph
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
        FLOAT m_fontSize;
        ITextureAllocator* m_pTextureAllocator;
};

