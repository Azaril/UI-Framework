#include "TextBox.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( Text );
DEFINE_GET_DEFAULT( AcceptsReturn, CBoolValue, TRUE );

//
// Properties
//
CStaticProperty CTextBox::TextProperty( L"Text", TypeIndex::String, StaticPropertyFlags::Content, &GET_DEFAULT(Text), &INSTANCE_CHANGE_CALLBACK( CTextBox, OnTextChanged ) );
CStaticProperty CTextBox::AcceptsReturnProperty( L"AcceptsReturn", TypeIndex::Bool, StaticPropertyFlags::None, &GET_DEFAULT(AcceptsReturn), &INSTANCE_CHANGE_CALLBACK( CTextBox, OnAcceptsReturnChanged ) );

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CTextBox, OnTextChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CTextBox, OnAcceptsReturnChanged );

CTextBox::CTextBox() : m_TextEditor(NULL),
                       m_TextLayout(NULL),
                       m_TextHostControl(NULL),
                       m_TextHost(NULL),
                       m_AcceptsReturn(this, &CTextBox::AcceptsReturnProperty)
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

    IFC(CTextEditor::Create(&m_TextEditor));

Cleanup:
    return hr;
}

HRESULT CTextBox::OnAttach(CUIAttachContext& Context)
{
    HRESULT hr = S_OK;
    CTextProvider* pTextProvider = NULL;
    SizeF InitialSize;

    IFC(m_VisualContext.GetGraphicsDevice()->GetTextProvider(&pTextProvider));

    IFC(pTextProvider->CreateEditableTextLayout(InitialSize, &m_TextLayout));

    IFC(CControl::OnAttach(Context));

Cleanup:
    ReleaseObject(pTextProvider);

    return hr;
}

HRESULT CTextBox::OnDetach(CUIDetachContext& Context)
{
    HRESULT hr = S_OK;

    IFC(CControl::OnDetach(Context));

    ReleaseObject(m_TextLayout);

Cleanup:
    return hr;
}

HRESULT CTextBox::PostTemplateApplied()
{
    HRESULT hr = S_OK;

    IFC(CControl::PostTemplateApplied());

    IFC(GetTemplateChild(L"PART_TextHost", &m_TextHostControl));

    IFCPTR(m_TextHostControl);

    IFC(CTextHost::Create(GetProviders(), m_TextLayout, &m_TextHost));

    IFC(m_TextHostControl->SetContent(m_TextHost));

    IFC(m_TextEditor->SetTextHost(this));

    IFC(m_TextEditor->SetTextLayout(m_TextLayout));

Cleanup:
    return hr;
}

HRESULT CTextBox::PreTemplateRevoked()
{
    HRESULT hr = S_OK;

    if(m_TextHostControl)
    {
        IFC(m_TextHostControl->SetContent(NULL));
    }

    ReleaseObject(m_TextHostControl);

    IFC(m_TextHost->SetTextLayout(NULL));

    IFC(m_TextEditor->SetTextLayout(NULL));

    IFC(m_TextEditor->SetTextHost(this));

    IFC(CControl::PreTemplateRevoked());

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
    CDelegatingPropertyInformation* pDelegatingPropertyInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &TextProperty,
        &AcceptsReturnProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));
    IFC(CControl::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingPropertyInformation));

    *ppInformation = pDelegatingPropertyInformation;
    pDelegatingPropertyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingPropertyInformation);

    return hr;
}

HRESULT CTextBox::GetLayeredValue(CProperty* pProperty, CLayeredValue** ppLayeredValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    //TODO: Make this a lookup table rather than requiring a comparison per property.
    if(pProperty == &CTextBox::AcceptsReturnProperty)
    {
        *ppLayeredValue = &m_AcceptsReturn;
    }
    else
    {
        hr = CControl::GetLayeredValue(pProperty, ppLayeredValue);
    }

Cleanup:
    return hr;
}

HRESULT CTextBox::SetValueInternal(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;
    CStringValue* pStringValue = NULL;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    if(pProperty == &CTextBox::TextProperty)
    {
        IFC(CastType(pValue, &pStringValue));

        IFC(m_TextEditor->SetText(pStringValue->GetValue(), pStringValue->GetLength()));
    }
    else
    {
        IFC(CControl::SetValueInternal(pProperty, pValue));
    }

Cleanup:
    return hr;
}

HRESULT CTextBox::GetValueInternal(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;
    CStringValue* pText = NULL;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(pProperty == &CTextBox::TextProperty)
    {
        IFC(CStringValue::Create(m_TextEditor->GetText(), &pText));
        
        *ppValue = pText;
        pText = NULL;
    }
    else
    {
        IFC(CControl::GetValueInternal(pProperty, ppValue));
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

        IFC(m_TextEditor->SetText(pTextValue->GetValue(), pTextValue->GetLength()));
    }
    else
    {
        IFC(m_TextEditor->SetText(NULL, 0))
    }

    //TODO: Update layout, or query if the value is from the layout?

Cleanup:
    return hr;
}

HRESULT CTextBox::OnAcceptsReturnChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;
    CBoolValue* pBool = NULL;

    IFC(CastType(pNewValue, &pBool));

    if(pBool)
    {
        m_TextEditor->SetAcceptsEnter(pBool->GetValue());
    }

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

//
// CTextBox
//
extern "C" __declspec(dllexport)
TypeIndex::Value CTextBox_TypeIndex()
{
    return TypeIndex::TextBox;
}

extern "C" __declspec(dllexport)
CControl* CTextBox_CastTo_CControl(CTextBox* pTextBox)
{
    return pTextBox;
}

extern "C" __declspec(dllexport)
CTextBox* CObjectWithType_CastTo_CTextBox(CObjectWithType* pObject)
{
    return (pObject->IsTypeOf(TypeIndex::TextBox)) ? (CTextBox*)pObject : NULL;
}

extern "C" __declspec(dllexport)
CProperty* CTextBox_GetTextProperty()
{
    return &CTextBox::TextProperty;
}