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

    for (map< UINT32, ITexture* >::iterator it = m_GlyphTextures.begin(); it != m_GlyphTextures.end(); ++it)
    {
        if (it->second != NULL)
        {
            it->second->Release();
        }
    }

    for (map< UINT32, GlyphMetrics* >::iterator it = m_GlyphMetrics.begin(); it != m_GlyphMetrics.end(); ++it)
    {
        delete it->second;
    }
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
CFreetypeTextFormat::GetGlyphTexture(
    const UINT32 glyph,
    __deref_out_opt ITexture** ppTexture
    )
{
    HRESULT hr = S_OK;
    ITexture* pNewTexture = NULL;
    ITexture* pTexture = NULL;

    map< UINT32, ITexture* >::iterator it = m_GlyphTextures.find(glyph);

    if (it != m_GlyphTextures.end())
    {
        pTexture = it->second;
    }
    else
    {
        IFC(m_pFontFace->LoadIntoTexture(glyph, m_fontSize, m_pTextureAllocator, &pNewTexture));

        m_GlyphTextures.insert(std::make_pair(glyph, pNewTexture));

        pTexture = pNewTexture;
        pNewTexture = NULL;
    }

    SetObject(*ppTexture, pTexture);

Cleanup:
    ReleaseObject(pNewTexture);

    return hr;
}

__checkReturn HRESULT
CFreetypeTextFormat::GetGlyphMetics(
    const UINT32 glyph,
    __deref_out const GlyphMetrics** ppGlyphMetrics
    )
{
    HRESULT hr = S_OK;
    GlyphMetrics* pMetrics = NULL;

    map< UINT32, GlyphMetrics* >::const_iterator it = m_GlyphMetrics.find(glyph);

    if (it != m_GlyphMetrics.end())
    {
        *ppGlyphMetrics = it->second;
    }
    else
    {
        pMetrics = new GlyphMetrics();
        IFCOOM(pMetrics);

        IFC(m_pFontFace->GetGlyphMetrics(glyph, m_fontSize, pMetrics));

        m_GlyphMetrics.insert(std::make_pair(glyph, pMetrics));

        *ppGlyphMetrics = pMetrics;
        pMetrics = NULL;
    }

Cleanup:
    delete pMetrics;

    return hr;
}