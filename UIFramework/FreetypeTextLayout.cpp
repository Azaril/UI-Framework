#include "FreetypeTextLayout.h"

CFreetypeTextLayout::CFreetypeTextLayout(
    )
    : m_pTextFormat(NULL)
    , m_LayoutValid(FALSE)
    , m_pLayoutEngine(NULL)
{
}

CFreetypeTextLayout::~CFreetypeTextLayout(
    )
{
    ReleaseObject(m_pLayoutEngine);
    ReleaseObject(m_pTextFormat);
}

__checkReturn HRESULT 
CFreetypeTextLayout::Initialize(
    __in_ecount(characterCount) const WCHAR* pText,
    UINT32 characterCount,
    __in CFreetypeTextFormat* pTextFormat,
    const SizeF& size
    )
{
    HRESULT hr = S_OK;

    SetObject(m_pTextFormat, pTextFormat);

    IFC(CTextLayoutEngine::Create(this, &m_pLayoutEngine));

    IFC(m_pLayoutEngine->SetText(pText, characterCount));

    IFC(SetMaxSize(size));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CFreetypeTextLayout::SetMaxSize(
    const SizeF& Size 
    )
{
    HRESULT hr = S_OK;

    IFC(m_pLayoutEngine->SetMaxSize(Size));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CFreetypeTextLayout::GetMetrics(
    __deref_out CTextLayoutMetrics** ppMetrics 
    )
{
    HRESULT hr = S_OK;

    IFC(m_pLayoutEngine->GetMetrics(ppMetrics));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CFreetypeTextLayout::GetFontMetrics(
    __deref_out const FontMetrics** ppFontMetrics
    )
{
    HRESULT hr = S_OK;

    IFC(m_pTextFormat->GetFontMetrics(ppFontMetrics));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CFreetypeTextLayout::GetGlyphMetrics(
    UINT32 glyph,
    __deref_out const GlyphMetrics** ppGlyphMetrics
    )
{
    HRESULT hr = S_OK;

    IFC(m_pTextFormat->GetGlyphMetics(glyph, ppGlyphMetrics));

Cleanup:
    return hr;
}

struct RenderContext
{
    ITextLayoutRenderCallback* Callback;
    void* Context;
};

__checkReturn HRESULT
CFreetypeTextLayout::Render(
    __in ITextLayoutRenderCallback* pCallback,
    __in_opt void* pContext
    )
{
    HRESULT hr = S_OK;

    {
        RenderContext context = { pCallback, pContext };

        IFC(m_pLayoutEngine->Render(this, &context));
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT
CFreetypeTextLayout::RenderGlyphRun( 
    UINT32 glyph, 
    __in GlyphRun* pGlyphRun,
    __in_opt void* pContext
    )
{
    HRESULT hr = S_OK;
    ITexture* pGlyphTexture = NULL;

    IFC(m_pTextFormat->GetGlyphTexture(glyph, &pGlyphTexture));

    if (pGlyphTexture != NULL)
    {
        RenderContext* pRenderContext = (RenderContext*)pContext;

        IFC(pRenderContext->Callback->RenderGlyphRun(pGlyphTexture, pGlyphRun, pRenderContext->Context));
    }

Cleanup:
    ReleaseObject(pGlyphTexture);

    return hr;
}

__override UINT32
CFreetypeTextLayout::GetStartPosition(
    )
{
    HRESULT hr = S_OK;

    return hr;
}

__override UINT32 
CFreetypeTextLayout::GetEndPosition(
    )
{
    HRESULT hr = S_OK;

    return hr;
}

__override __checkReturn HRESULT 
CFreetypeTextLayout::SetText( 
    __in_ecount_opt(TextLength) const WCHAR* pText, 
    UINT32 TextLength 
    )
{
    HRESULT hr = S_OK;

    IFC(m_pLayoutEngine->SetText(pText, TextLength));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT
CFreetypeTextLayout::ClearText(
    )
{
    HRESULT hr = S_OK;

    IFC(m_pLayoutEngine->SetText(NULL, 0));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CFreetypeTextLayout::InsertText(
    UINT32 Position, 
    __in_ecount(TextLength) const WCHAR* pText,
    UINT32 TextLength
    )
{
    HRESULT hr = S_OK;

    IFC(m_pLayoutEngine->InsertText(Position, pText, TextLength));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT
CFreetypeTextLayout::RemoveText( 
    UINT32 Position, 
    UINT32 Length 
    )
{
    HRESULT hr = S_OK;

    IFC(m_pLayoutEngine->RemoveText(Position, Length));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT
CFreetypeTextLayout::GetText(
    __deref_out_ecount(*pTextLength) const WCHAR** ppText,
    __out UINT32* pTextLength
    )
{
    HRESULT hr = S_OK;

    IFC(m_pLayoutEngine->GetText(ppText, pTextLength));

Cleanup:
    return hr;
}