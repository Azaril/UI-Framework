#include "TextLayoutEngine.h"

CTextLayoutEngine::CTextLayoutEngine(
    )
    : m_pCallback(NULL)
    , m_pText(NULL)
    , m_TextLength(0)
{
}

CTextLayoutEngine::~CTextLayoutEngine(
    )
{
    ClearGlyphRuns();

    delete [] m_pText;
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

    m_TextLength = characterCount;

    m_pText = new WCHAR[m_TextLength];
    IFCOOM(m_pText);

    memcpy(m_pText, pText, m_TextLength * sizeof(WCHAR));

Cleanup:
    return hr;
}

__checkReturn HRESULT
CTextLayoutEngine::Layout(
    const SizeF& maxSize
    )
{
    HRESULT hr = S_OK;
    Point2U currentPosition;
    RectU bounds;

    ClearGlyphRuns();

    for (UINT32 i = 0; i < m_TextLength; ++i)
    {
        const GlyphMetrics* pMetrics = NULL;

        IFC(m_pCallback->GetGlyphMetrics(m_pText[i], &pMetrics));

        {
            GlyphData data;

            Point2U renderPosition = currentPosition + pMetrics->Offset;
            Point2U bottomRight = currentPosition + pMetrics->Offset + pMetrics->Size;

            bounds.right = std::max(bottomRight.x, bounds.right);
            bounds.bottom = std::max(bottomRight.y, bounds.bottom);

            data.Position.x = renderPosition.x  / 64.0f;
            data.Position.y = renderPosition.y  / 64.0f;

            GlyphRun* pGlyphRun = NULL;

            IFC(GetGlyphRun(m_pText[i], &pGlyphRun));

            pGlyphRun->GlyphData.push_back(data);
        }

        currentPosition.x += pMetrics->Advance.x;
        //TODO: Enable vertical layout.
        //currentPosition.x += pMetrics->Advance.y;
    }

    IFC(m_pCallback->SetBounds(RectF(bounds.left / 64.0f, bounds.top / 64.0f, bounds.right / 64.0f, bounds.bottom / 64.0f)));

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

    for (map< UINT32, GlyphRun* >::iterator it = m_GlyphRuns.begin(); it != m_GlyphRuns.end(); ++it)
    {
        IFC(pCallback->RenderGlyphRun(it->first, it->second, pContext));
    }

Cleanup:
    return hr;
}