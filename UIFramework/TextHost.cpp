#include "TextHost.h"

CTextHost::CTextHost() : m_TextVisual(NULL),
                         m_TextLayout(NULL),
                         m_TextFormat(NULL)
{
}

CTextHost::~CTextHost()
{
    ReleaseObject(m_TextVisual);
    ReleaseObject(m_TextLayout);
    ReleaseObject(m_TextFormat);
}

HRESULT CTextHost::Initialize(CProviders* pProviders, CTextLayout* pTextLayout)
{
    HRESULT hr = S_OK;

    IFC(CFrameworkElement::Initialize(pProviders));

    IFC(CTextVisual::Create(GetProviders(), &m_TextVisual));

    IFC(AddChildVisual(m_TextVisual));

    IFC(SetTextLayout(pTextLayout));

Cleanup:
    return hr;
}

HRESULT CTextHost::SetTextLayout(CTextLayout* pLayout)
{
    HRESULT hr = S_OK;

    ReplaceObject(m_TextLayout, pLayout);

    IFC(m_TextVisual->SetTextLayout(pLayout));

    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

bool CTextHost::ShouldClipToLayout()
{
    HRESULT hr = S_OK;
    CTextLayoutMetrics* pTextLayoutMetrics = NULL;   
    RectF TextBounds;

    bool ClipToBounds = CUIElement::ShouldClipToLayout();

    if(!ClipToBounds && m_TextLayout != NULL)
    {
        IFC(m_TextLayout->GetMetrics(&pTextLayoutMetrics));

        IFC(pTextLayoutMetrics->GetBounds(&TextBounds));

        ClipToBounds = !MakeRect(GetFinalSize()).IsFullyContained(TextBounds);
    }

Cleanup:
    ReleaseObject(pTextLayoutMetrics);

    return ClipToBounds;
}

__override __checkReturn HRESULT 
CTextHost::MeasureInternal( 
    const SizeF& AvailableSize,
    SizeF& DesiredSize
    )
{
    HRESULT hr = S_OK;
    CTextLayoutMetrics* pTextLayoutMetrics = NULL;
    
    RectF TextBounds;
    SizeF SizeWithText;

    if(m_TextLayout)
    {
        IFC(m_TextLayout->SetMaxSize(AvailableSize));

        IFC(m_TextLayout->GetMetrics(&pTextLayoutMetrics));

        IFC(pTextLayoutMetrics->GetBounds(&TextBounds));
    }

    DesiredSize.width = TextBounds.right;
    DesiredSize.height = TextBounds.bottom;

Cleanup:
    ReleaseObject(pTextLayoutMetrics);

    return hr;
}

__override __checkReturn HRESULT 
CTextHost::ArrangeInternal(
    const SizeF& AvailableSize,
    SizeF& UsedSize 
    )
{
    HRESULT hr = S_OK;
    RectF TextBounds;

    if(m_TextLayout)
    {
        IFC(m_TextLayout->SetMaxSize(AvailableSize));
    }

    UsedSize = AvailableSize;

Cleanup:
    return hr;
}

HRESULT CTextHost::HitTest(Point2F LocalPoint, CHitTestResult** ppHitTestResult)
{
    HRESULT hr = S_OK;
    SizeF RenderSize;

    IFCPTR(ppHitTestResult);

    RenderSize = GetFinalSize();

    if(LocalPoint.x >= 0 && LocalPoint.y >= 0 && LocalPoint.x <= RenderSize.width && LocalPoint.y <= RenderSize.height)
    {
        IFC(CHitTestResult::Create(this, ppHitTestResult));
    }
    else
    {
        *ppHitTestResult = NULL;
    }

Cleanup:
    return hr;
}