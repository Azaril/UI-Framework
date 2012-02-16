#include "TextBox.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"
#include "AssertUtil.h"

//
// Properties
//
namespace TextBoxProperties
{
    enum Value
    {
        Text,
        AcceptsReturn
    };
}

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( Text );
DEFINE_GET_DEFAULT( AcceptsReturn, bool, TRUE );

//
// Properties
//
CStaticProperty CTextBox::TextProperty(TextBoxProperties::Text, L"Text", TypeIndex::String, StaticPropertyFlags::Content, &GET_DEFAULT(Text), &INSTANCE_CHANGE_CALLBACK( CTextBox, OnTextChanged ));
CStaticProperty CTextBox::AcceptsReturnProperty(TextBoxProperties::AcceptsReturn, L"AcceptsReturn", TypeIndex::Bool, StaticPropertyFlags::None, &GET_DEFAULT(AcceptsReturn), &INSTANCE_CHANGE_CALLBACK( CTextBox, OnAcceptsReturnChanged ));

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CTextBox, OnTextChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CTextBox, OnAcceptsReturnChanged );

CTextBox::CTextBox() : m_TextEditor(NULL),
                       m_TextLayout(NULL),
                       m_TextHostControl(NULL),
                       m_TextHost(NULL),
                       m_TextFormat(NULL),
                       m_AcceptsReturn(this, &CTextBox::AcceptsReturnProperty)
{
}

CTextBox::~CTextBox()
{
    ReleaseObject(m_TextFormat);
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
    CTextFormat* pTextFormat = NULL;

    IFC(CControl::OnAttach(Context));

    IFC(EnsureTextLayout());

Cleanup:
    ReleaseObject(pTextProvider);
    ReleaseObject(pTextFormat);

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

__checkReturn HRESULT
CTextBox::EnsureTextLayout(
    )
{
    HRESULT hr = S_OK;
    CTextProvider* pTextProvider = NULL;
    SizeF InitialSize;

    if(m_TextLayout == NULL)
    {
        IFC(m_VisualContext.GetGraphicsDevice()->GetTextProvider(&pTextProvider));

        if(m_TextFormat == NULL)
        {
            const CFontDescription* pFontDescription = GetEffectiveFontDescription();

            //TODO: Handle font description changes.
            IFC(pTextProvider->CreateFormat(pFontDescription, &m_TextFormat));
        }

        IFC(pTextProvider->CreateEditableTextLayout(m_TextFormat, InitialSize, &m_TextLayout));
    }

Cleanup:
    ReleaseObject(pTextProvider);

    return hr;
}

HRESULT CTextBox::GetTextLayout(
    CEditableTextLayout** ppLayout
    )
{
    HRESULT hr = S_OK;
    
    IFC(EnsureTextLayout());

    SetObject(*ppLayout, m_TextLayout);

Cleanup:
    return hr;
}

HRESULT CTextBox::PostTemplateApplied()
{
    HRESULT hr = S_OK;
    CEditableTextLayout* pTextLayout = NULL;

    IFC(CControl::PostTemplateApplied());

    IFC(GetTemplateChild(L"PART_TextHost", &m_TextHostControl));

    IFCPTR(m_TextHostControl);

    IFC(GetTextLayout(&pTextLayout));

    IFC(CTextHost::Create(GetProviders(), pTextLayout, &m_TextHost));

    IFC(m_TextHostControl->SetContent(m_TextHost));

    IFC(m_TextEditor->SetTextHost(this));

    IFC(m_TextEditor->SetTextLayout(pTextLayout));

Cleanup:
    ReleaseObject(pTextLayout);

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

    ReleaseObject(m_TextHost);

    IFC(CControl::PreTemplateRevoked());

Cleanup:
    return hr;
}

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

    IFCPTR(pProperty);
    IFCPTR(pValue);

    if(pProperty == &CTextBox::TextProperty)
    {
        //NOTE: Don't do anything here, the value changed notification will propogate 
        //      the value to the layout.
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
        const WCHAR* pContainedText = NULL;
        UINT32 textLength = 0;

        IFC(m_TextEditor->GetText(&pContainedText, &textLength));

        IFC(CStringValue::Create(pContainedText, textLength, &pText));
        
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
extern "C" UIFRAMEWORK_API
TypeIndex::Value CTextBox_TypeIndex()
{
    return TypeIndex::TextBox;
}

extern "C" UIFRAMEWORK_API
CControl* CTextBox_CastTo_CControl(CTextBox* pTextBox)
{
    return pTextBox;
}

extern "C" UIFRAMEWORK_API
CTextBox* CObjectWithType_CastTo_CTextBox(CObjectWithType* pObject)
{
    return (pObject->IsTypeOf(TypeIndex::TextBox)) ? (CTextBox*)pObject : NULL;
}

extern "C" UIFRAMEWORK_API
CProperty* CTextBox_GetTextProperty()
{
    return &CTextBox::TextProperty;
}