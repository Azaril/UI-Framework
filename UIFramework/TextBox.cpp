#include "TextBox.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( Text );

//
// Properties
//
CStaticProperty CTextBox::TextProperty( L"Text", TypeIndex::String, StaticPropertyFlags::Content, &GET_DEFAULT(Text), &INSTANCE_CHANGE_CALLBACK( CTextBox, OnTextChanged ) );

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CTextBox, OnTextChanged );

CTextBox::CTextBox() : m_TextEditor(NULL),
                       m_TextLayout(NULL),
                       m_TextHostControl(NULL),
                       m_TextHost(NULL),
                       m_Text(this, &CTextBox::TextProperty)
{
}

CTextBox::~CTextBox()
{
    ReleaseObject(m_TextEditor);
    ReleaseObject(m_TextLayout);
    ReleaseObject(m_TextHostControl);
    ReleaseObject(m_TextHost);
}

HRESULT CTextBox::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFC(CControl::Initialize(pProviders));

Cleanup:
    return hr;
}

HRESULT CTextBox::OnAttach(CUIAttachContext& Context)
{
    HRESULT hr = S_OK;
    CTextProvider* pTextProvider = NULL;
    SizeF InitialSize = { 0 };
    CStringValue* pText = NULL;

    IFC(CControl::OnAttach(Context));

    IFC(m_VisualContext.GetGraphicsDevice()->GetTextProvider(&pTextProvider));

    IFC(pTextProvider->CreateEditableTextLayout(InitialSize, &m_TextLayout));

    IFC(CTextEditor::Create(this, m_TextLayout, &m_TextEditor));

    IFC(m_Text.GetTypedEffectiveValue(GetProviders(), &pText));

    if(pText)
    {
        IFC(m_TextLayout->SetText(pText->GetValue(), pText->GetLength()));
    }

    IFC(GetTemplateChild(L"PART_TextHost", &m_TextHostControl));

    IFCPTR(m_TextHostControl);

    IFC(CTextHost::Create(GetProviders(), m_TextLayout, &m_TextHost));

    IFC(m_TextHostControl->SetContent(m_TextHost));

Cleanup:
    ReleaseObject(pTextProvider);
    ReleaseObject(pText);

    return hr;
}

HRESULT CTextBox::OnDetach(CUIDetachContext& Context)
{
    HRESULT hr = S_OK;

    if(m_TextHostControl)
    {
        IFC(m_TextHostControl->SetContent(NULL));
    }

    ReleaseObject(m_TextHostControl);

    IFC(m_TextHost->SetTextLayout(NULL));

    IFC(CControl::OnDetach(Context));

    //TODO: Preserve text and layout so it can be reloaded if attached again.
    ReleaseObject(m_TextLayout);
    ReleaseObject(m_TextEditor);

Cleanup:
    return hr;
}

//HRESULT CTextBox::MeasureInternal(SizeF AvailableSize, SizeF& DesiredSize)
//{
//    HRESULT hr = S_OK;
//    CTextLayoutMetrics* pTextLayoutMetrics = NULL;
//    
//    RectF TextBounds = { 0 };
//    SizeF SizeWithText = { 0 };
//
//    IFC(m_TextLayout->SetMaxSize(AvailableSize));
//
//    IFC(m_TextLayout->GetMetrics(&pTextLayoutMetrics));
//
//    IFC(pTextLayoutMetrics->GetBounds(&TextBounds));
//
//    DesiredSize.width = TextBounds.right - TextBounds.left;
//    DesiredSize.height = TextBounds.bottom - TextBounds.top;
//
//Cleanup:
//    ReleaseObject(pTextLayoutMetrics);
//
//    return hr;
//}

//HRESULT CTextBox::ArrangeInternal(SizeF AvailableSize, SizeF& UsedSize)
//{
//    HRESULT hr = S_OK;
//    CTextLayoutMetrics* pMetrics = NULL;
//    RectF TextBounds = { 0 };
//
//    IFC(m_TextLayout->SetMaxSize(AvailableSize));
//
//    IFC(m_TextLayout->GetMetrics(&pMetrics));
//
//    IFC(pMetrics->GetBounds(&TextBounds));
//
//    UsedSize.width = TextBounds.right - TextBounds.left;
//    UsedSize.height = TextBounds.bottom - TextBounds.top;
//
//Cleanup:
//    ReleaseObject(pMetrics);
//
//    return hr;
//}

HRESULT CTextBox::CreatePropertyInformation(CPropertyInformation** ppInformation)
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
    IFC(CControl::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingProperyInformation));

    *ppInformation = pDelegatingProperyInformation;
    pDelegatingProperyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingProperyInformation);

    return hr;
}

HRESULT CTextBox::GetLayeredValue(CProperty* pProperty, CLayeredValue** ppLayeredValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    //TODO: Make this a lookup table rather than requiring a comparison per property.
    if(pProperty == &CTextBox::TextProperty)
    {
        *ppLayeredValue = &m_Text;
    }
    else
    {
        hr = CControl::GetLayeredValue(pProperty, ppLayeredValue);
    }

Cleanup:
    return hr;
}

HRESULT CTextBox::OnTextChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    IFC(InvalidateMeasure());

    if(pNewValue)
    {
        CStringValue* pTextValue = NULL;

        IFC(CastType(pNewValue, &pTextValue));

        if(m_TextLayout)
        {
            IFC(m_TextLayout->SetText(pTextValue->GetValue(), pTextValue->GetLength()));
        }
    }
    else
    {
        if(m_TextLayout)
        {
            IFC(m_TextLayout->ClearText());
        }
    }

    //TODO: Update layout, or query if the value is from the layout?

Cleanup:
    return hr;
}

HRESULT CTextBox::HitTest(Point2F LocalPoint, CHitTestResult** ppHitTestResult)
{
    HRESULT hr = S_OK;
    IFCPTR(ppHitTestResult);

    *ppHitTestResult = NULL;

Cleanup:
    return hr;
}