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
                           m_TextVisual(NULL),
                           m_Text(this, &CTextBlock::TextProperty),
                           m_Foreground(this, &CTextBlock::ForegroundProperty)
{
}

CTextBlock::~CTextBlock()
{
    ReleaseObject(m_TextLayout);
    ReleaseObject(m_TextFormat);
    ReleaseObject(m_TextVisual);
}

HRESULT CTextBlock::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFC(CFrameworkElement::Initialize(pProviders));

    IFC(CTextVisual::Create(&m_TextVisual));

    IFC(AddChildVisual(m_TextVisual));

Cleanup:
    return hr;
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

HRESULT CTextBlock::InvalidateTextLayout()
{
    HRESULT hr = S_OK;

    ReleaseObject(m_TextLayout);

    IFC(m_TextVisual->SetTextLayout(NULL));

Cleanup:
    return hr;
}

HRESULT CTextBlock::GetTextLayout(CTextLayout** ppLayout)
{
    HRESULT hr = S_OK;
    CTextProvider* pTextProvider = NULL;
    CStringValue* pText = NULL;
    CTextFormat* pTextFormat = NULL;

    IFCPTR(ppLayout);

    if(m_TextLayout == NULL)
    {
        IFC(GetEffectiveText(&pText));

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

        if(pText != NULL)
        {
            IFC(pTextProvider->CreateTextLayout(pText->GetValue(), wcslen(pText->GetValue()), pTextFormat, GetDesiredSize(), &m_TextLayout));
        }
        else
        {
            IFC(pTextProvider->CreateTextLayout(NULL, 0, pTextFormat, GetDesiredSize(), &m_TextLayout));
        }

        IFC(m_TextVisual->SetTextLayout(m_TextLayout));
    }

    *ppLayout = m_TextLayout;
    AddRefObject(m_TextLayout);

Cleanup:
    ReleaseObject(pText);
    ReleaseObject(pTextFormat);
    ReleaseObject(pTextProvider);

    return hr;
}

HRESULT CTextBlock::MeasureInternal(SizeF AvailableSize, SizeF& DesiredSize)
{
    HRESULT hr = S_OK;
    CTextLayout* pTextLayout = NULL;
    CTextLayoutMetrics* pTextLayoutMetrics = NULL;
    
    RectF TextBounds;
    SizeF SizeWithText;

    IFC(GetTextLayout(&pTextLayout));

    IFC(pTextLayout->SetMaxSize(AvailableSize));

    IFC(pTextLayout->GetMetrics(&pTextLayoutMetrics));

    IFC(pTextLayoutMetrics->GetBounds(&TextBounds));

    DesiredSize.width = TextBounds.right - TextBounds.left;
    DesiredSize.height = TextBounds.bottom - TextBounds.top;

Cleanup:
    ReleaseObject(pTextLayout);
    ReleaseObject(pTextLayoutMetrics);

    return hr;
}

HRESULT CTextBlock::ArrangeInternal(SizeF AvailableSize, SizeF& UsedSize)
{
    HRESULT hr = S_OK;
    CTextLayout* pTextLayout = NULL;
    CTextLayoutMetrics* pMetrics = NULL;
    RectF TextBounds;

    IFC(GetTextLayout(&pTextLayout));

    IFC(pTextLayout->SetMaxSize(AvailableSize));

    IFC(pTextLayout->GetMetrics(&pMetrics));

    IFC(pMetrics->GetBounds(&TextBounds));

    UsedSize = AvailableSize;
    //UsedSize.width = TextBounds.right - TextBounds.left;
    //UsedSize.height = TextBounds.bottom - TextBounds.top;

Cleanup:
    ReleaseObject(pTextLayout);
    ReleaseObject(pMetrics);

    return hr;
}

HRESULT CTextBlock::CreatePropertyInformation(CPropertyInformation** ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingPropertyInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &TextProperty,
        &ForegroundProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));
    IFC(CFrameworkElement::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingPropertyInformation));

    *ppInformation = pDelegatingPropertyInformation;
    pDelegatingPropertyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingPropertyInformation);

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

    IFC(InvalidateTextLayout());

    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CTextBlock::GetEffectiveText(CStringValue** ppText)
{
    HRESULT hr = S_OK;

    IFCPTR(ppText);

    IFC(m_Text.GetTypedEffectiveValue(ppText));

Cleanup:
    return hr;
}

HRESULT CTextBlock::OnForegroundChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;
    CBrush* pBrush = NULL;

    IFC(CastType(pNewValue, &pBrush));

    IFC(m_TextVisual->SetForegroundBrush(pBrush));

Cleanup:
    return hr;
}

HRESULT CTextBlock::GetEffectiveForeground(CBrush** ppBrush)
{
    HRESULT hr = S_OK;

    IFCPTR(ppBrush);

    IFC(m_Foreground.GetTypedEffectiveValue(ppBrush));

Cleanup:
    return hr;
}

HRESULT CTextBlock::HitTest(Point2F LocalPoint, CHitTestResult** ppHitTestResult)
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

//
// CTextBlock
//
extern "C" __declspec(dllexport)
TypeIndex::Value CTextBlock_TypeIndex()
{
    return TypeIndex::TextBlock;
}

extern "C" __declspec(dllexport)
CFrameworkElement* CTextBlock_CastTo_CFrameworkElement(CTextBlock* pTextBlock)
{
    return pTextBlock;
}

extern "C" __declspec(dllexport)
CTextBlock* CObjectWithType_CastTo_CTextBlock(CObjectWithType* pObject)
{
    return (pObject->IsTypeOf(TypeIndex::TextBlock)) ? (CTextBlock*)pObject : NULL;
}

extern "C" __declspec(dllexport)
CProperty* CTextBlock_GetTextProperty()
{
    return &CTextBlock::TextProperty;
}