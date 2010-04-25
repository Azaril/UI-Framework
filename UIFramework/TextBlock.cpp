#include "TextBlock.h"
#include "TextProvider.h"

CTextBlock::CTextBlock() : m_TextLayout(NULL),
                           m_TextFormat(NULL),
                           m_TextBrush(NULL)
{
}

CTextBlock::~CTextBlock()
{
    ReleaseObject(m_TextLayout);
    ReleaseObject(m_TextFormat);
    ReleaseObject(m_TextBrush);
}

HRESULT CTextBlock::SetText(const WCHAR* pText)
{
    HRESULT hr = S_OK;

    IFCPTR(pText);

    m_Text = pText;

Cleanup:
    return hr;
}

HRESULT CTextBlock::MeasureInternal(SizeF AvailableSize, SizeF& DesiredSize)
{
    HRESULT hr = S_OK;
    SizeF BaseDesiredSize = { 0 };
    CTextProvider* pTextProvider = NULL;
    CTextLayoutMetrics* pTextLayoutMetrics = NULL;
    CTextFormat* pTextFormat = NULL;
    RectF TextBounds = { 0 };
    SizeF SizeWithText = { 0 };

    IFC(CFrameworkElement::MeasureInternal(AvailableSize, BaseDesiredSize));

    if(!m_Text.empty())
    {
        IFC(m_VisualContext.GetGraphicsDevice()->GetTextProvider(&pTextProvider));

        if(m_TextFormat == NULL)
        {
            IFC(pTextProvider->GetDefaultFormat(&pTextFormat));
        }
        else
        {
            pTextFormat = m_TextFormat;
            AddRefObject(m_TextFormat);
        }

        if(m_TextLayout == NULL)
        {
            IFC(pTextProvider->CreateTextLayout(m_Text.c_str(), m_Text.length(), pTextFormat, AvailableSize, &m_TextLayout));
        }
        else
        {
            IFC(m_TextLayout->SetMaxSize(AvailableSize));
        }

        IFC(m_TextLayout->GetMetrics(&pTextLayoutMetrics));

        IFC(pTextLayoutMetrics->GetBounds(&TextBounds));

        DesiredSize.width = max(TextBounds.right - TextBounds.left, BaseDesiredSize.width);
        DesiredSize.height = max(TextBounds.bottom - TextBounds.top, BaseDesiredSize.height);
    }
    else
    {
        DesiredSize = BaseDesiredSize;
    }

Cleanup:
    ReleaseObject(pTextProvider);
    ReleaseObject(pTextLayoutMetrics);
    ReleaseObject(pTextFormat);

    return hr;
}

HRESULT CTextBlock::Arrange(SizeF Size)
{
    HRESULT hr = S_OK;

    if(m_TextLayout != NULL)
    {
        IFC(m_TextLayout->SetMaxSize(Size));
    }

    IFC(CFrameworkElement::Arrange(Size));

Cleanup:
    return hr;
}

HRESULT CTextBlock::RenderTransformed(CRenderContext& Context)
{
    HRESULT hr = S_OK;
    CRenderTarget* pRenderTarget = NULL;
    Point2F TextOrigin = { 0 };
    CBrush* pTextBrush = NULL;

    IFC(CFrameworkElement::RenderTransformed(Context));

    if(m_TextLayout != NULL)
    {
        pRenderTarget = Context.GetRenderTarget();
        IFCPTR(pRenderTarget);

        if(m_TextBrush == NULL)
        {
            IFC(pRenderTarget->CreateSolidBrush(D2D1::ColorF(D2D1::ColorF::Black), &pTextBrush));
        }
        else
        {
            pTextBrush = m_TextBrush;
            AddRefObject(m_TextBrush);
        }

        IFC(pRenderTarget->RenderTextLayout(TextOrigin, m_TextLayout, pTextBrush));  
    }

Cleanup:
    ReleaseObject(m_TextBrush);

    return hr;
}