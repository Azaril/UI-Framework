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

    //TODO: Implement!
    IFC(CFreetypeTextLayoutMetics::Create(MakeRect(m_Size), &m_pLayoutMetrics));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT
CFreetypeTextLayout::BeginGlyphQueries(
    )
{
    HRESULT hr = S_OK;

    IFC(m_pTextFormat->BeginQuery());

Cleanup:
    return hr;
}

__override __checkReturn HRESULT
CFreetypeTextLayout::EndGlyphQueries(
    )
{
    HRESULT hr = S_OK;

    IFC(m_pTextFormat->EndQuery());

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CFreetypeTextLayout::GetGlyph(
    WCHAR glyph
    )
{
    HRESULT hr = S_OK;

    IFC(m_pTextFormat->LoadGlyph(glyph));

Cleanup:
    return hr;
}