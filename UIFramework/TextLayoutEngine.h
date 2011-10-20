#pragma once

#include "Factory.h"
#include "RefCounted.h"
#include "GlyphMetrics.h"
#include "GlyphRun.h"

struct ITextLayoutCallback
{
    virtual __checkReturn HRESULT GetGlyphMetrics(
        UINT32 glyph,
        __deref_out const GlyphMetrics** ppGlyphMetrics
        ) = 0;

    virtual __checkReturn HRESULT SetBounds(
        const RectF& bounds
        ) = 0;
};

struct ITextLayoutEngineRenderCallback
{
    virtual __checkReturn HRESULT RenderGlyphRun(
        UINT32 glyph,
        __in GlyphRun* pGlyphRun,
        __in_opt void* pContext
        ) = 0;
};

class CTextLayoutEngine : public CRefCountedObject
{
    public:
        DECLARE_FACTORY3( CTextLayoutEngine, const WCHAR*, UINT32, ITextLayoutCallback* );

        __checkReturn HRESULT Layout(
            const SizeF& maxSize
            );

        __checkReturn HRESULT Render(
            __in ITextLayoutEngineRenderCallback* pCallback,
            __in_opt void* pContext
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

        __checkReturn HRESULT GetGlyphRun(
            UINT32 glyph,
            __deref_out GlyphRun** ppGlyphRun
            );

        void
        ClearGlyphRuns(
            );

        ITextLayoutCallback* m_pCallback;
        WCHAR* m_pText;
        UINT32 m_TextLength;
        map< UINT32, GlyphRun* > m_GlyphRuns;
};

