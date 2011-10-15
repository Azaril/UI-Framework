#pragma once

#include "Factory.h"
#include "RefCounted.h"
#include "TextLayout.h"
#include "FreetypeTextFormat.h"
#include "FreetypeTextLayoutMetics.h"
#include "TextLayoutEngine.h"
#include "TextureAllocator.h"

class CFreetypeTextLayout : public CTextLayout,
                            private ITextLayoutCallback
{
    public:
        DECLARE_FACTORY4( CFreetypeTextLayout, const WCHAR*, UINT32, CFreetypeTextFormat*, const SizeF& );

        __override virtual __checkReturn HRESULT SetMaxSize(
            const SizeF& Size 
            );

        __override virtual __checkReturn HRESULT GetMetrics(
            __deref_out CTextLayoutMetrics** ppMetrics 
            );

    protected:
        CFreetypeTextLayout(
            );

        virtual ~CFreetypeTextLayout(
            );

        __checkReturn HRESULT Initialize(
            __in_ecount(characterCount) const WCHAR* pText,
            UINT32 characterCount,
            __in CFreetypeTextFormat* pTextFormat,
            const SizeF& size
            );

        __checkReturn HRESULT InvalidateLayout(
            );

        __checkReturn HRESULT EnsureLayout(
            );

        __override virtual __checkReturn HRESULT BeginGlyphQueries(
            );

        __override virtual __checkReturn HRESULT GetGlyph(
            WCHAR glyph
            );

        __override virtual __checkReturn HRESULT EndGlyphQueries(
            );

        CFreetypeTextFormat* m_pTextFormat;
        SizeF m_Size;
        BOOL m_LayoutValid;
        CFreetypeTextLayoutMetics* m_pLayoutMetrics;
        CTextLayoutEngine* m_pLayoutEngine;
};

