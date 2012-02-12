#include "FreetypeTextFormat.h"

CFreetypeTextFormat::CFreetypeTextFormat(
    )
    : m_pFontFace(NULL)
    , m_fontSize(0)
    , m_pTextureAllocator(NULL)
    , m_HaveFontMetrics(FALSE)
    , m_pFontSize(NULL)
{
}

CFreetypeTextFormat::~CFreetypeTextFormat(
    )
{
    ReleaseObject(m_pFontSize);

    if (m_pFontFace != NULL)
    {
        m_pFontFace->RemoveCachedFormat(this);
    }

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

    IFC(m_pFontFace->CreateFontSize(fontSize, &m_pFontSize));

Cleanup:
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
        IFC(m_pFontFace->LoadIntoTexture(glyph, m_pFontSize, m_pTextureAllocator, &pNewTexture));

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
CFreetypeTextFormat::GetFontMetrics(
    __deref_out const FontMetrics** ppFontMetrics
    )
{
    HRESULT hr = S_OK;

    if (!m_HaveFontMetrics)
    {
        IFC(m_pFontSize->GetFontMetrics(&m_FontMetrics));

        m_HaveFontMetrics = TRUE;
    }

    *ppFontMetrics = &m_FontMetrics;

Cleanup:
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

        IFC(m_pFontFace->GetGlyphMetrics(glyph, m_pFontSize, pMetrics));

        m_GlyphMetrics.insert(std::make_pair(glyph, pMetrics));

        *ppGlyphMetrics = pMetrics;
        pMetrics = NULL;
    }

Cleanup:
    delete pMetrics;

    return hr;
}

__checkReturn HRESULT 
CFreetypeTextFormat::GetSupportsKerning(
    __out bool* pSupportsKerning
    )
{
    HRESULT hr = S_OK;

    IFC(m_pFontFace->GetSupportsKerning(pSupportsKerning));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CFreetypeTextFormat::GetKerning(
    UINT32 leftGlyph,
    UINT32 rightGlyph,
    __out Point2I* pKerning
    )
{
    HRESULT hr = S_OK;

    IFC(m_pFontFace->GetKerning(leftGlyph, rightGlyph, m_pFontSize, pKerning));

Cleanup:
    return hr;
}

FLOAT
CFreetypeTextFormat::GetFontSize(
    )
{
    return m_fontSize;
}