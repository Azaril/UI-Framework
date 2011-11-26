#pragma once

#include "Factory.h"
#include "RefCounted.h"
#include "TextLayout.h"
#include "TextLayoutBase.h"
#include "FreetypeTextFormat.h"
#include "TextLayoutEngine.h"
#include "TextureAllocator.h"
#include "EditableTextLayoutBase.h"

class CFreetypeTextLayout : public CEditableTextLayoutBase,
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

        __override virtual UINT32 GetStartPosition(
            );

        __override virtual UINT32 GetEndPosition(
            );

        __override virtual __checkReturn HRESULT SetText( 
            __in_ecount_opt(TextLength) const WCHAR* pText, 
            UINT32 TextLength 
            );

        __override virtual __checkReturn HRESULT ClearText(
            );

        __override virtual __checkReturn HRESULT InsertText(
            UINT32 Position, 
            __in_ecount(TextLength) const WCHAR* pText,
            UINT32 TextLength
            );

        __override virtual __checkReturn HRESULT RemoveText( 
            UINT32 Position, 
            UINT32 Length 
            );

        __override virtual __checkReturn HRESULT GetText(
            __deref_out_ecount(*pTextLength) const WCHAR** ppText,
            __out UINT32* pTextLength
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

        __override virtual __checkReturn HRESULT GetFontMetrics(
            __deref_out const FontMetrics** ppFontMetrics
            );

        __override virtual __checkReturn HRESULT GetGlyphMetrics(
            UINT32 glyph,
            __deref_out const GlyphMetrics** ppGlyphMetrics
            );

        __override virtual __checkReturn HRESULT RenderGlyphRun(
            UINT32 glyph,
            __in GlyphRun* pGlyphRun,
            __in_opt void* pContext
            );

        CFreetypeTextFormat* m_pTextFormat;
        SizeF m_Size;
        bool m_LayoutValid;
        CTextLayoutEngine* m_pLayoutEngine;
        RectF m_Bounds;
};

