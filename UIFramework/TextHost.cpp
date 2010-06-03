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

    IFC(CTextVisual::Create(&m_TextVisual));

    IFC(AddChildVisual(m_TextVisual));

    IFC(SetTextLayout(pTextLayout));

Cleanup:
    return hr;
}

HRESULT CTextHost::SetTextLayout(CTextLayout* pLayout)
{
    HRESULT hr = S_OK;

    ReleaseObject(m_TextLayout);

    m_TextLayout = pLayout;

    AddRefObject(m_TextLayout);

    IFC(m_TextVisual->SetTextLayout(pLayout));

    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CTextHost::MeasureInternal(SizeF AvailableSize, SizeF& DesiredSize)
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

    DesiredSize.width = TextBounds.right - TextBounds.left;
    DesiredSize.height = TextBounds.bottom - TextBounds.top;

Cleanup:
    ReleaseObject(pTextLayoutMetrics);

    return hr;
}

HRESULT CTextHost::ArrangeInternal(SizeF AvailableSize, SizeF& UsedSize)
{
    HRESULT hr = S_OK;
    CTextLayoutMetrics* pMetrics = NULL;
    RectF TextBounds;

    if(m_TextLayout)
    {
        IFC(m_TextLayout->SetMaxSize(AvailableSize));

        IFC(m_TextLayout->GetMetrics(&pMetrics));

        IFC(pMetrics->GetBounds(&TextBounds));
    }

    UsedSize.width = TextBounds.right - TextBounds.left;
    UsedSize.height = TextBounds.bottom - TextBounds.top;

Cleanup:
    ReleaseObject(pMetrics);

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