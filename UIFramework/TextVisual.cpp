#include "TextVisual.h"

CTextVisual::CTextVisual() : m_ForegroundBrush(NULL),
                             m_ForegroundGraphicsBrush(NULL),
                             m_TextLayout(NULL)
{
}

CTextVisual::~CTextVisual()
{
    ReleaseObject(m_ForegroundBrush);
    ReleaseObject(m_ForegroundGraphicsBrush);
    ReleaseObject(m_TextLayout);
}

HRESULT CTextVisual::Initialize(CTextLayout* pTextLayout)
{
    HRESULT hr = S_OK;

    IFC(CVisual::Initialize());

    IFC(SetTextLayout(pTextLayout));

Cleanup:
    return hr;
}

HRESULT CTextVisual::SetForegroundBrush(CBrush* pBrush)
{
    HRESULT hr = S_OK;

    ReleaseObject(m_ForegroundBrush);
    ReleaseObject(m_ForegroundGraphicsBrush);

    m_ForegroundBrush = pBrush;

    AddRefObject(m_ForegroundBrush);

    return hr;
}

HRESULT CTextVisual::SetTextLayout(CTextLayout* pTextLayout)
{
    HRESULT hr = S_OK;

    ReleaseObject(m_TextLayout);

    m_TextLayout = pTextLayout;

    AddRefObject(m_TextLayout);

    return hr;
}

HRESULT CTextVisual::PreRender(CPreRenderContext& Context)
{
    HRESULT hr = S_OK;
    CRenderTarget* pRenderTarget = NULL;

    pRenderTarget = Context.GetRenderTarget();
    IFCPTR(pRenderTarget);

    if(m_TextLayout != NULL && m_ForegroundGraphicsBrush == NULL)
    {
        if(m_ForegroundBrush)
        {
            if(FAILED(m_ForegroundBrush->GetGraphicsBrush(m_VisualContext.GetGraphicsDevice(), pRenderTarget, &m_ForegroundGraphicsBrush)))
            {
                m_ForegroundGraphicsBrush = NULL;
            }
        }

        if(m_ForegroundGraphicsBrush == NULL)
        {
            IFC(pRenderTarget->GetDefaultBrush(DefaultBrush::TextForeground, &m_ForegroundGraphicsBrush));
        }
    }

    IFC(CVisual::PreRender(Context));

Cleanup:
    return hr;
}

HRESULT CTextVisual::RenderTransformed(CRenderContext& Context)
{
    HRESULT hr = S_OK;
    CRenderTarget* pRenderTarget = NULL;
    Point2F TextOrigin = { 0 };

    pRenderTarget = Context.GetRenderTarget();
    IFCPTR(pRenderTarget);

    IFC(CVisual::RenderTransformed(Context));

    if(m_TextLayout != NULL && m_ForegroundGraphicsBrush != NULL)
    {
        IFC(pRenderTarget->RenderTextLayout(TextOrigin, m_TextLayout, m_ForegroundGraphicsBrush));  
    }

Cleanup:
    return hr;
}

HRESULT CTextVisual::HitTest(Point2F LocalPoint, CHitTestResult** ppHitTestResult)
{
    HRESULT hr = S_OK;
    SizeF RenderSize = { 0 };

    IFCPTR(ppHitTestResult);

    //TODO: Implement hit testing.

    *ppHitTestResult = NULL;

Cleanup:
    return hr;
}