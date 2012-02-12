#include "TextLayoutEngine.h"

CTextLayoutEngine::CTextLayoutEngine(
    )
    : m_pCallback(NULL)
    , m_LayoutDirty(TRUE)
    , m_pLayoutMetrics(NULL)
{
}

CTextLayoutEngine::~CTextLayoutEngine(
    )
{
    ClearGlyphRuns();

    ReleaseObject(m_pLayoutMetrics);
}

__checkReturn HRESULT
CTextLayoutEngine::Initialize(
    __in ITextLayoutCallback* pCallback
    )
{
    HRESULT hr = S_OK;

    m_pCallback = pCallback;

    return hr;
}

__checkReturn HRESULT
CTextLayoutEngine::SetText(
    __in_ecount_opt(TextLength) const WCHAR* pText, 
    UINT32 TextLength
    )
{
    HRESULT hr = S_OK;

    //TODO: Make this more efficient.
    ClearGlyphRuns();

    m_Text.assign(pText, TextLength);

    IFC(InvalidateLayout());

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CTextLayoutEngine::InsertText(
    UINT32 Position, 
    __in_ecount(TextLength) const WCHAR* pText,
    UINT32 TextLength
    )
{
    HRESULT hr = S_OK;

    m_Text.insert(Position, pText, TextLength);

    IFC(InvalidateLayout());

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CTextLayoutEngine::RemoveText( 
    UINT32 Position, 
    UINT32 Length 
    )
{
    HRESULT hr = S_OK;

    m_Text.erase(Position, Length);

    IFC(InvalidateLayout());

Cleanup:
    return hr;
}

__checkReturn HRESULT
CTextLayoutEngine::GetText(
    __deref_out_ecount(*pTextLength) const WCHAR** ppText,
    __out UINT32* pTextLength    
    )
{
    HRESULT hr = S_OK;

    *ppText = m_Text.c_str();
    *pTextLength = m_Text.length();

    return hr;
}

__checkReturn HRESULT
CTextLayoutEngine::InvalidateLayout(
    )
{
    HRESULT hr = S_OK;

    m_LayoutDirty = TRUE;

    return hr;
}

__checkReturn HRESULT
CTextLayoutEngine::EnsureLayout(
    )
{
    HRESULT hr = S_OK;
    Point2I currentPosition;
    Point2I linePosition;
    RectU bounds;
    const FontMetrics* pFontMetrics = NULL;

    if (m_LayoutDirty)
    {
        ClearGlyphRuns();

        IFC(m_pCallback->GetFontMetrics(&pFontMetrics));

        linePosition.y += pFontMetrics->Asecender;
        currentPosition = linePosition;

        bounds.bottom += pFontMetrics->Height >> 6;

        {
            UINT32 previousGlyph = 0;
            bool useKerning = false;

            IFC(m_pCallback->GetSupportsKerning(&useKerning));

            for (UINT32 i = 0; i < m_Text.length(); ++i)
            {
                const GlyphMetrics* pMetrics = NULL;
                const UINT32 glyph = m_Text[i];

                if (m_Text[i] == L'\n')
                {
                    linePosition.y += pFontMetrics->Height;

                    currentPosition = linePosition;
                }
                else
                {
                    IFC(m_pCallback->GetGlyphMetrics(glyph, &pMetrics));

                    if (useKerning && previousGlyph != 0)
                    {
                        Point2I kerningData;

                        //
                        // NOTE: First parameter is visual left glyph, second parameter is visual right.
                        //       For right to left the ordering is important and may need to be switched.
                        //
                        if (SUCCEEDED(m_pCallback->GetKerning(glyph, previousGlyph, &kerningData)))
                        {
                            currentPosition.x += kerningData.x;
                            currentPosition.y -= kerningData.y;
                        }
                    }

                    {
                        GlyphData data;

                        Point2I renderPosition = currentPosition;

                        renderPosition.x += pMetrics->HorizontalBearing.x;
                        renderPosition.y -= pMetrics->HorizontalBearing.y;

                        Point2I bottomRight = renderPosition + pMetrics->Size;

                        bounds.right = std::max((UINT32)bottomRight.x, bounds.right);
                        bounds.bottom = std::max((UINT32)bottomRight.y, bounds.bottom);

                        //
                        // Convert from 26.6 fixed point to floating point.
                        //
                        data.Position.x = renderPosition.x / 64.0f;
                        data.Position.y = renderPosition.y / 64.0f;

                        GlyphRun* pGlyphRun = NULL;

                        IFC(GetGlyphRun(glyph, &pGlyphRun));

                        pGlyphRun->Glyphs.push_back(data);
                    }

                    currentPosition.x += pMetrics->HorizontalAdvance;
                }

                previousGlyph = glyph;
            }
        }

        ReleaseObject(m_pLayoutMetrics);

        //
        // Convert from 26.6 fixed point to floating point.
        //
        IFC(CTextLayoutEngineMetrics::Create(RectF(bounds.left / 64.0f, bounds.top / 64.0f, bounds.right / 64.0f, bounds.bottom / 64.0f), &m_pLayoutMetrics));

        m_LayoutDirty = FALSE;
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT
CTextLayoutEngine::GetGlyphRun(
    const UINT32 glyph,
    __deref_out GlyphRun** ppGlyphRun
    )
{
    HRESULT hr = S_OK;
    GlyphRun* pRun = NULL;

    map< UINT32, GlyphRun* >::iterator it = m_GlyphRuns.find(glyph);

    if (it != m_GlyphRuns.end())
    {
        pRun = it->second;
    }

    if (pRun == NULL)
    {
        pRun = new GlyphRun();
        IFCOOM(pRun);

        m_GlyphRuns.insert(std::make_pair(glyph, pRun));
    }

    *ppGlyphRun = pRun;

Cleanup:
    return hr;
}

__checkReturn HRESULT
CTextLayoutEngine::SetMaxSize(
    const SizeF& size
    )
{
    HRESULT hr = S_OK;

    if (m_LayoutSize != size)
    {
        m_LayoutSize = size;

        IFC(InvalidateLayout());
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT
CTextLayoutEngine::GetMetrics( 
    __deref_out CTextLayoutMetrics** ppMetrics
    )
{
    HRESULT hr = S_OK;

    IFC(EnsureLayout());

    SetObject(*ppMetrics, m_pLayoutMetrics);

Cleanup:
    return hr;
}

void
CTextLayoutEngine::ClearGlyphRuns(
    )
{
    for (map< UINT32, GlyphRun* >::iterator it = m_GlyphRuns.begin(); it != m_GlyphRuns.end(); ++it)
    {
        delete it->second;
    }

    m_GlyphRuns.clear();
}

__checkReturn HRESULT
CTextLayoutEngine::Render(
    __in ITextLayoutEngineRenderCallback* pCallback, 
    __in_opt void* pContext 
    )
{
    HRESULT hr = S_OK;

    IFC(EnsureLayout());

    for (map< UINT32, GlyphRun* >::iterator it = m_GlyphRuns.begin(); it != m_GlyphRuns.end(); ++it)
    {
        IFC(pCallback->RenderGlyphRun(it->first, it->second, pContext));
    }

Cleanup:
    return hr;
}