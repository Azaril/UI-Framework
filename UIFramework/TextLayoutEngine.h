#pragma once

#include "Factory.h"
#include "RefCounted.h"

struct ITextLayoutCallback
{
    virtual __checkReturn HRESULT BeginGlyphQueries(
        ) = 0;

    virtual __checkReturn HRESULT GetGlyph(
        WCHAR glyph
        ) = 0;

    virtual __checkReturn HRESULT EndGlyphQueries(
        ) = 0;
};

class CTextLayoutEngine : public CRefCountedObject
{
    public:
        DECLARE_FACTORY3( CTextLayoutEngine, const WCHAR*, UINT32, ITextLayoutCallback* );

        __checkReturn HRESULT Layout(
            const SizeF& maxSize
            );

    protected:
        CTextLayoutEngine(
            );

        virtual ~CTextLayoutEngine(
            );

        __checkReturn HRESULT Initialize(
            __in_ecount(characterCount) const WCHAR* pText,
            UINT32 characterCount,
            __in ITextLayoutCallback* pCallback
            );

        __checkReturn HRESULT LoadGlyphs(
            __in_ecount(characterCount) const WCHAR* pText,
            UINT32 characterCount
            );

        ITextLayoutCallback* m_pCallback;
};

