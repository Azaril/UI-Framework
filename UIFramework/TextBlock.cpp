#include "TextBlock.h"
#include "TextProvider.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"
#include "BasicTypes.h"

CStaticProperty TextBlockProperties[] = 
{
    CStaticProperty( L"Text", TypeIndex::String, StaticPropertyFlags::Content )
};

namespace TextBlockPropertyIndex
{
    enum Value
    {
        Text
    };
}

CTextBlock::CTextBlock() : m_TextLayout(NULL),
                           m_TextFormat(NULL),
                           m_TextBrush(NULL),
                           m_TextGraphicsBrush(NULL)
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

    IFCPTR(pText);

    m_Text = pText;

    ReleaseObject(m_TextLayout);

    IFC(InvalidateMeasure());

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

    IFC(CStaticPropertyInformation::Create(TextBlockProperties, ARRAYSIZE(TextBlockProperties), &pStaticInformation));
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

HRESULT CTextBlock::SetValue(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    // Check if the property is a static property.
    if(pProperty >= TextBlockProperties && pProperty < TextBlockProperties + ARRAYSIZE(TextBlockProperties))
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        UINT32 Index = (pStaticProperty - TextBlockProperties);
        
        switch(Index)
        {
            case TextBlockPropertyIndex::Text:
                {
                    IFCEXPECT(pValue->IsTypeOf(TypeIndex::String));

                    CStringValue* pText = (CStringValue*)pValue;

                    IFC(SetText(pText->GetValue()));

                    break;
                }

            default:
                {
                    IFC(E_FAIL);
                }
        }
    }
    else
    {
        IFC(CFrameworkElement::SetValue(pProperty, pValue));
    }

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