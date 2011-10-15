#include "TextLayoutEngine.h"

CTextLayoutEngine::CTextLayoutEngine(
    )
    : m_pCallback(NULL)
{
}

CTextLayoutEngine::~CTextLayoutEngine(
    )
{
}

__checkReturn HRESULT
CTextLayoutEngine::Initialize(
    __in_ecount(characterCount) const WCHAR* pText,
    UINT32 characterCount,
    __in ITextLayoutCallback* pCallback
    )
{
    HRESULT hr = S_OK;

    m_pCallback = pCallback;

    IFC(LoadGlyphs(pText, characterCount));

Cleanup:
    return hr;
}

__checkReturn HRESULT
CTextLayoutEngine::LoadGlyphs(
    __in_ecount(characterCount) const WCHAR* pText,
    UINT32 characterCount
    )
{
    HRESULT hr = S_OK;

    IFC(m_pCallback->BeginGlyphQueries());

    for (UINT32 i = 0; i < characterCount; ++i)
    {
        IFC(m_pCallback->GetGlyph(pText[i]));
    }

    IFC(m_pCallback->EndGlyphQueries());

Cleanup:
    return hr;
}

__checkReturn HRESULT
CTextLayoutEngine::Layout(
    const SizeF& maxSize
    )
{
    HRESULT hr = S_OK;

    return hr;
}