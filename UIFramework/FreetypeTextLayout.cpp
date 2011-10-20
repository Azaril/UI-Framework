#include "FreetypeTextLayout.h"

CFreetypeTextLayout::CFreetypeTextLayout(
    )
    : m_pTextFormat(NULL)
    , m_LayoutValid(FALSE)
    , m_pLayoutMetrics(NULL)
    , m_pLayoutEngine(NULL)
{
}

CFreetypeTextLayout::~CFreetypeTextLayout(
    )
{
    ReleaseObject(m_pLayoutEngine);
    ReleaseObject(m_pTextFormat);
    ReleaseObject(m_pLayoutMetrics);
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

    IFC(CTextLayoutEngine::Create(pText, characterCount, this, &m_pLayoutEngine));

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

    if (m_Size != Size)
    {
        m_Size = Size;

        InvalidateLayout();
    }

    return hr;
}

__override __checkReturn HRESULT 
CFreetypeTextLayout::GetMetrics(
    __deref_out CTextLayoutMetrics** ppMetrics 
    )
{
    HRESULT hr = S_OK;

    IFC(EnsureLayout());

    SetObject(*ppMetrics, m_pLayoutMetrics);

Cleanup:
    return hr;
}

__checkReturn HRESULT
CFreetypeTextLayout::InvalidateLayout(
    )
{
    HRESULT hr = S_OK;

    m_LayoutValid = FALSE;

    ReleaseObject(m_pLayoutMetrics);

    return hr;
}

__checkReturn HRESULT
CFreetypeTextLayout::EnsureLayout(
    )
{
    HRESULT hr = S_OK;

    IFC(m_pLayoutEngine->Layout(m_Size));

    IFC(CFreetypeTextLayoutMetics::Create(m_Bounds, &m_pLayoutMetrics));

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

__override __checkReturn HRESULT
CFreetypeTextLayout::SetBounds(
    const RectF& bounds
    )
{
    HRESULT hr = S_OK;

    m_Bounds = bounds;

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