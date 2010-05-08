#include "TextBlock.h"
#include "TextProvider.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"
#include "BasicTypes.h"

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( Text );

//
// Properties
// 
CStaticProperty CTextBlock::TextProperty( L"Text", TypeIndex::String, StaticPropertyFlags::Content, &GET_DEFAULT( Text ), &INSTANCE_CHANGE_CALLBACK( CTextBlock, OnTextChanged ) );

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CTextBlock, OnTextChanged );

CTextBlock::CTextBlock() : m_TextLayout(NULL),
                           m_TextFormat(NULL),
                           m_TextBrush(NULL),
                           m_TextGraphicsBrush(NULL),
                           m_Text(this, &CTextBlock::TextProperty)
{
}

CTextBlock::~CTextBlock()
{
    ReleaseObject(m_TextLayout);
    ReleaseObject(m_TextFormat);
    ReleaseObject(m_TextBrush);
    ReleaseObject(m_TextGraphicsBrush);
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
    SizeF BaseDesiredSize = { 0 };
    CTextProvider* pTextProvider = NULL;
    CTextLayoutMetrics* pTextLayoutMetrics = NULL;
    CTextFormat* pTextFormat = NULL;
    RectF TextBounds = { 0 };
    SizeF SizeWithText = { 0 };
    CStringValue* pText = NULL;

    IFC(CFrameworkElement::MeasureInternal(AvailableSize, BaseDesiredSize));

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
    ReleaseObject(pText);

    return hr;
}

HRESULT CTextBlock::ArrangeInternal(SizeF Size)
{
    HRESULT hr = S_OK;

    if(m_TextLayout != NULL)
    {
        IFC(m_TextLayout->SetMaxSize(Size));
    }

    IFC(CFrameworkElement::ArrangeInternal(Size));

Cleanup:
    return hr;
}

HRESULT CTextBlock::RenderTransformed(CRenderContext& Context)
{
    HRESULT hr = S_OK;
    CRenderTarget* pRenderTarget = NULL;
    Point2F TextOrigin = { 0 };
    CGraphicsBrush* pTextBrush = NULL;

    IFC(CFrameworkElement::RenderTransformed(Context));

    if(m_TextLayout != NULL)
    {
        pRenderTarget = Context.GetRenderTarget();
        IFCPTR(pRenderTarget);

        if(m_TextBrush == NULL)
        {
            IFC(pRenderTarget->GetDefaultBrush(DefaultBrush::TextForeground, &pTextBrush));
        }
        else
        {
            //TODO: Figure out brush semantics...
            __debugbreak();

            IFC(m_TextBrush->GetGraphicsBrush(m_VisualContext.GetGraphicsDevice(), pRenderTarget, &pTextBrush));

            //pTextBrush = m_TextBrush;
            //AddRefObject(m_TextBrush);
        }

        IFC(pRenderTarget->RenderTextLayout(TextOrigin, m_TextLayout, pTextBrush));  
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
        &TextProperty
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

HRESULT CTextBlock::HitTest(Point2F LocalPoint, CHitTestResult** ppHitTestResult)
{
    HRESULT hr = S_OK;

    IFCPTR(ppHitTestResult);

    *ppHitTestResult = NULL;

Cleanup:
    return hr;
}