#include "FreetypeTextFormat.h"

CFreetypeTextFormat::CFreetypeTextFormat(
    )
    : m_pFontFace(NULL)
    , m_fontSize(0)
    , m_pTextureAllocator(NULL)
{
}

CFreetypeTextFormat::~CFreetypeTextFormat(
    )
{
    ReleaseObject(m_pFontFace);
}

__checkReturn HRESULT 
CFreetypeTextFormat::Initialize(
    __in CFreetypeFontFace* pFontFace,
    FLOAT fontSize,
    __in ITextureAllocator* pTextureAllocator
    )
{
    HRESULT hr = S_OK;

    SetObject(m_pFontFace, pFontFace);

    m_fontSize = fontSize;
    m_pTextureAllocator = pTextureAllocator;

    return hr;
}

__checkReturn HRESULT
CFreetypeTextFormat::BeginQuery( 
    )
{
    HRESULT hr = S_OK;

    IFC(m_pFontFace->SetFontSize(m_fontSize));

Cleanup:
    return hr;
}

__checkReturn HRESULT
CFreetypeTextFormat::EndQuery(
    )
{
    HRESULT hr = S_OK;

    return hr;
}

__checkReturn HRESULT
CFreetypeTextFormat::LoadGlyph( 
    const WCHAR glyph
    )
{
    HRESULT hr = S_OK;

    IFC(m_pFontFace->LoadGlyph(glyph));

    //HACK: Testing hack!
    {
        ITexture* pTexture = NULL;

        IFC(m_pFontFace->LoadIntoTexture(m_pTextureAllocator, &pTexture));
    }

Cleanup:
    return hr;
}