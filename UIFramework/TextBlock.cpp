#include "TextBlock.h"
#include "TextProvider.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"
#include "BasicTypes.h"

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( Text );
DEFINE_GET_DEFAULT_NULL( Foreground );

//
// Properties
// 
CStaticProperty CTextBlock::TextProperty( L"Text", TypeIndex::String, StaticPropertyFlags::Content, &GET_DEFAULT( Text ), &INSTANCE_CHANGE_CALLBACK( CTextBlock, OnTextChanged ) );
CStaticProperty CTextBlock::ForegroundProperty( L"Foreground", TypeIndex::Brush, StaticPropertyFlags::None, &GET_DEFAULT( Foreground ), &INSTANCE_CHANGE_CALLBACK( CTextBlock, OnForegroundChanged ) );

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CTextBlock, OnTextChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CTextBlock, OnForegroundChanged );

CTextBlock::CTextBlock() : m_TextLayout(NULL),
                           m_TextFormat(NULL),
                           m_ForegroundGraphicsBrush(NULL),
                           m_Text(this, &CTextBlock::TextProperty),
                           m_Foreground(this, &CTextBlock::ForegroundProperty)
{
}

CTextBlock::~CTextBlock()
{
    ReleaseObject(m_TextLayout);
    ReleaseObject(m_TextFormat);
    ReleaseObject(m_ForegroundGraphicsBrush);
}

HRESULT CTextBlock::SetText(const WCHAR* pText)
{
    HRESULT hr = S_OK;
    CStringValue* pString = NULL;

    IFCPTR(pText);

    IFC(CStringValue::Create(pText, &pString));

    IFC(SetValue(&CTextBlock::TextProperty, pString));

Cleanup:
    ReleaseObject(pString);

    return hr;
}

HRESULT CTextBlock::MeasureInternal(SizeF AvailableSize, SizeF& DesiredSize)
{
    HRESULT hr = S_OK;
    CTextProvider* pTextProvider = NULL;
    CTextLayoutMetrics* pTextLayoutMetrics = NULL;
    CTextFormat* pTextFormat = NULL;
    RectF TextBounds = { 0 };
    SizeF SizeWithText = { 0 };
    CStringValue* pText = NULL;

    IFC(GetEffectiveText(&pText));

    if(pText)
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
            IFC(pTextProvider->CreateTextLayout(pText->GetValue(), wcslen(pText->GetValue()), pTextFormat, AvailableSize, &m_TextLayout));
        }
        else
        {
            IFC(m_TextLayout->SetMaxSize(AvailableSize));
        }

        IFC(m_TextLayout->GetMetrics(&pTextLayoutMetrics));

        IFC(pTextLayoutMetrics->GetBounds(&TextBounds));

        DesiredSize.width = TextBounds.right - TextBounds.left;
        DesiredSize.height = TextBounds.bottom - TextBounds.top;
    }
    else
    {
        DesiredSize.width = 0;
        DesiredSize.height = 0;
    }

Cleanup:
    ReleaseObject(pTextProvider);
    ReleaseObject(pTextLayoutMetrics);
    ReleaseObject(pTextFormat);
    ReleaseObject(pText);

    return hr;
}

HRESULT CTextBlock::ArrangeInternal(SizeF AvailableSize, SizeF& UsedSize)
{
    HRESULT hr = S_OK;
    CTextLayoutMetrics* pMetrics = NULL;
    RectF TextBounds = { 0 };

    if(m_TextLayout != NULL)
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

HRESULT CTextBlock::PreRenderInternal(CPreRenderContext& Context)
{
    HRESULT hr = S_OK;
    CRenderTarget* pRenderTarget = NULL;
    CBrush* pBrush = NULL;

    pRenderTarget = Context.GetRenderTarget();
    IFCPTR(pRenderTarget);

    if(m_ForegroundGraphicsBrush == NULL)
    {
        IFC(GetEffectiveForeground(&pBrush));

        if(pBrush)
        {
            if(FAILED(pBrush->GetGraphicsBrush(m_VisualContext.GetGraphicsDevice(), pRenderTarget, &m_ForegroundGraphicsBrush)))
            {
                m_ForegroundGraphicsBrush = NULL;
            }
        }

        if(m_ForegroundGraphicsBrush == NULL)
        {
            IFC(pRenderTarget->GetDefaultBrush(DefaultBrush::TextForeground, &m_ForegroundGraphicsBrush));
        }
    }

    IFC(CFrameworkElement::PreRenderInternal(Context));

Cleanup:
    ReleaseObject(pBrush);

    return hr;
}

HRESULT CTextBlock::RenderTransformed(CRenderContext& Context)
{
    HRESULT hr = S_OK;
    CRenderTarget* pRenderTarget = NULL;
    Point2F TextOrigin = { 0 };
    CGraphicsBrush* pTextBrush = NULL;

    pRenderTarget = Context.GetRenderTarget();
    IFCPTR(pRenderTarget);

    IFC(CFrameworkElement::RenderTransformed(Context));

    if(m_TextLayout != NULL && m_ForegroundGraphicsBrush != NULL)
    {
        IFC(pRenderTarget->RenderTextLayout(TextOrigin, m_TextLayout, m_ForegroundGraphicsBrush));  
    }

Cleanup:
    ReleaseObject(pTextBrush);

    return hr;
}

HRESULT CTextBlock::CreatePropertyInformation(CPropertyInformation** ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingProperyInformation = NULL;

    IFCPTR(ppInformation);

    CStaticProperty* Properties[] = 
    {
        &TextProperty,
        &ForegroundProperty
    };

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));
    IFC(CFrameworkElement::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingProperyInformation));

    *ppInformation = pDelegatingProperyInformation;
    pDelegatingProperyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingProperyInformation);

    return hr;
}

HRESULT CTextBlock::GetLayeredValue(CProperty* pProperty, CLayeredValue** ppLayeredValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    //TODO: Make this a lookup table rather than requiring a comparison per property.
    if(pProperty == &CTextBlock::TextProperty)
    {
        *ppLayeredValue = &m_Text;
    }
    else if(pProperty == &CTextBlock::ForegroundProperty)
    {
        *ppLayeredValue = &m_Foreground;
    }
    else
    {
        hr = CFrameworkElement::GetLayeredValue(pProperty, ppLayeredValue);
    }

Cleanup:
    return hr;
}

HRESULT CTextBlock::OnTextChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue )
{
    HRESULT hr = S_OK;

    ReleaseObject(m_TextFormat);

    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CTextBlock::GetEffectiveText(CStringValue** ppText)
{
    HRESULT hr = S_OK;

    IFCPTR(ppText);

    IFC(m_Text.GetTypedEffectiveValue(GetProviders(), ppText));

Cleanup:
    return hr;
}

HRESULT CTextBlock::OnForegroundChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    ReleaseObject(m_ForegroundGraphicsBrush);

Cleanup:
    return hr;
}

HRESULT CTextBlock::GetEffectiveForeground(CBrush** ppBrush)
{
    HRESULT hr = S_OK;

    IFCPTR(ppBrush);

    IFC(m_Foreground.GetTypedEffectiveValue(GetProviders(), ppBrush));

Cleanup:
    return hr;
}

HRESULT CTextBlock::HitTest(Point2F LocalPoint, CHitTestResult** ppHitTestResult)
{
    HRESULT hr = S_OK;

    IFCPTR(ppHitTestResult);

    SizeF RenderSize = GetFinalSize();

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