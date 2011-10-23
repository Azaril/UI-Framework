#pragma once

#include "Factory.h"
#include "RefCounted.h"
#include "GlyphMetrics.h"
#include "GlyphRun.h"
#include "FontMetrics.h"
#include "TextLayoutEngineMetrics.h"

struct ITextLayoutCallback
{
    virtual __checkReturn HRESULT GetFontMetrics(
        __deref_out const FontMetrics** ppFontMetrics
        ) = 0;

    virtual __checkReturn HRESULT GetGlyphMetrics(
        UINT32 glyph,
        __deref_out const GlyphMetrics** ppGlyphMetrics
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
        DECLARE_FACTORY1( CTextLayoutEngine, ITextLayoutCallback* );

        __checkReturn HRESULT SetMaxSize(
            const SizeF& size
            );

        __checkReturn HRESULT SetText(
            __in_ecount_opt(TextLength) const WCHAR* pText, 
            UINT32 TextLength
            );

        __checkReturn HRESULT GetText(
            __deref_out_ecount(*pTextLength) const WCHAR** ppText,
            __out UINT32* pTextLength
            );

        __checkReturn HRESULT InsertText(
            UINT32 Position, 
            __in_ecount(TextLength) const WCHAR* pText,
            UINT32 TextLength
            );

        __checkReturn HRESULT RemoveText( 
            UINT32 Position, 
            UINT32 Length 
            );

        __checkReturn HRESULT Render(
            __in ITextLayoutEngineRenderCallback* pCallback,
            __in_opt void* pContext
            );

        __override virtual __checkReturn HRESULT GetMetrics(
            __deref_out CTextLayoutMetrics** ppMetrics 
            );

    protected:
        CTextLayoutEngine(
            );

        virtual ~CTextLayoutEngine(
            );

        __checkReturn HRESULT Initialize(
            __in ITextLayoutCallback* pCallback
            );

        __checkReturn HRESULT GetGlyphRun(
            UINT32 glyph,
            __deref_out GlyphRun** ppGlyphRun
            );

        void
        ClearGlyphRuns(
            );

        __checkReturn HRESULT EnsureLayout(
            );

        __checkReturn HRESULT InvalidateLayout(
            );

        ITextLayoutCallback* m_pCallback;
        std::wstring m_Text;
        SizeF m_LayoutSize;
        map< UINT32, GlyphRun* > m_GlyphRuns;
        BOOL m_LayoutDirty;
        CTextLayoutEngineMetrics* m_pLayoutMetrics;
};

