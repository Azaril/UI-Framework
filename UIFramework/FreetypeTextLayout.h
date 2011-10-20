#pragma once

#include "Factory.h"
#include "RefCounted.h"
#include "TextLayout.h"
#include "TextLayoutBase.h"
#include "FreetypeTextFormat.h"
#include "FreetypeTextLayoutMetics.h"
#include "TextLayoutEngine.h"
#include "TextureAllocator.h"

class CFreetypeTextLayout : public CTextLayoutBase,
                            private ITextLayoutCallback,
                            private ITextLayoutEngineRenderCallback
{
    public:
        DECLARE_FACTORY4( CFreetypeTextLayout, const WCHAR*, UINT32, CFreetypeTextFormat*, const SizeF& );

        __override virtual __checkReturn HRESULT SetMaxSize(
            const SizeF& Size 
            );

        __override virtual __checkReturn HRESULT GetMetrics(
            __deref_out CTextLayoutMetrics** ppMetrics 
            );

        __override virtual __checkReturn HRESULT Render(
            __in ITextLayoutRenderCallback* pCallback,
            __in_opt void* pContext
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

        __override virtual __checkReturn HRESULT GetGlyphMetrics(
            UINT32 glyph,
            __deref_out const GlyphMetrics** ppGlyphMetrics
            );

        __override virtual __checkReturn HRESULT SetBounds(
            const RectF& bounds
            );

        __override virtual __checkReturn HRESULT RenderGlyphRun(
            UINT32 glyph,
            __in GlyphRun* pGlyphRun,
            __in_opt void* pContext
            );

        CFreetypeTextFormat* m_pTextFormat;
        SizeF m_Size;
        BOOL m_LayoutValid;
        CFreetypeTextLayoutMetics* m_pLayoutMetrics;
        CTextLayoutEngine* m_pLayoutEngine;
        RectF m_Bounds;
};

