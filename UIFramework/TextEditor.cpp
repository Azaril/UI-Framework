#include "TextEditor.h"
#include "TextEventArgs.h"
#include "KeyboardEventArgs.h"

CTextEditor::CTextEditor(
	) 
	: m_Host(NULL)
	, m_Layout(NULL)
	, m_CaretPosition(0)
	, m_AcceptsEnter(TRUE)
{
}

CTextEditor::~CTextEditor(
	)
{
    IGNOREHR(SetTextHost(NULL));
    IGNOREHR(SetTextLayout(NULL));
}

__checkReturn HRESULT 
CTextEditor::Initialize(
	)
{
    HRESULT hr = S_OK;

    return hr;
}

__checkReturn HRESULT 
CTextEditor::SetTextHost(
	__in_opt CUIElement* pElement
	)
{
    HRESULT hr = S_OK;

    if(m_Host)
    {
        m_TextConnection.disconnect();
        m_KeyDownConnection.disconnect();
        m_KeyUpConnection.disconnect();
    }

    m_Host = pElement;
    
    if(m_Host)
    {
        IFC(m_Host->AddHandler(&CUIElement::TextEvent, bind(&CTextEditor::OnText, this, _1, _2), &m_TextConnection));
        IFC(m_Host->AddHandler(&CUIElement::KeyDownEvent, bind(&CTextEditor::OnKeyDown, this, _1, _2), &m_KeyDownConnection));
        IFC(m_Host->AddHandler(&CUIElement::KeyUpEvent, bind(&CTextEditor::OnKeyUp, this, _1, _2), &m_KeyUpConnection));
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CTextEditor::SetTextLayout(
	__in_opt CEditableTextLayout* pLayout
	)
{
    HRESULT hr = S_OK;

    if(m_Layout)
    {
        const WCHAR* pText = NULL;
        UINT32 textLength = 0;

        IFC(m_Layout->GetText(&pText, &textLength));

        m_TextHolder.assign(pText, textLength);

        ReleaseObject(m_Layout);
    }

    m_Layout = pLayout;
    
    if(m_Layout)
    {
        AddRefObject(m_Layout);

        IFC(SetText(m_TextHolder.c_str(), m_TextHolder.length()));

        m_TextHolder.clear();
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CTextEditor::SetText(
	__in_ecount_opt(TextLength) const WCHAR* pText, 
	UINT32 TextLength
	)
{
    HRESULT hr = S_OK;

    if(m_Layout != NULL)
    {
        IFC(m_Layout->SetText(pText, TextLength));
    }
    else
    {
        m_TextHolder.assign(pText, TextLength);
    }

    m_CaretPosition = TextLength;

Cleanup:
    return hr;
}

__checkReturn HRESULT
CTextEditor::GetText(
    __deref_out_ecount(*pTextLength) const WCHAR** ppText,
    __out UINT32* pTextLength
	)
{
    HRESULT hr = S_OK;

    if(m_Layout)
    {
        IFC(m_Layout->GetText(ppText, pTextLength));
    }
    else
    {
        *ppText = NULL;
        *pTextLength = 0;
    }

Cleanup:
    return hr;
}

void 
CTextEditor::OnText(
	__in CObjectWithType* pSender,
	__in CRoutedEventArgs* pRoutedEventArgs
	)
{
    HRESULT hr = S_OK;
    CTextEventArgs* pTextEventArgs = NULL;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

    IFC(CastType(pRoutedEventArgs, &pTextEventArgs));

    if(!pTextEventArgs->IsHandled())
    {
        IFC(HandleText(pTextEventArgs->GetText(), pTextEventArgs->GetTextLength()));

        pTextEventArgs->SetHandled(TRUE);
    }

Cleanup:
    ;
}

__checkReturn HRESULT 
CTextEditor::HandleText(
	__in_ecount(TextLength) const WCHAR* pText, 
	UINT32 TextLength
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pText);

    IFC(InsertText(m_CaretPosition, pText, TextLength));

Cleanup:
    return hr;
}

__checkReturn HRESULT
CTextEditor::InsertText(
	UINT32 StartPosition, 
	__in_ecount(TextLength) const WCHAR* pText, 
	UINT32 TextLength
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pText);

    IFCPTR(m_Layout);

    IFC(m_Layout->InsertText(StartPosition, pText, TextLength));

    if(m_CaretPosition >= StartPosition)
    {
        m_CaretPosition += TextLength;
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CTextEditor::RemoveText(
	UINT32 StartPosition, 
	UINT32 Length
	)
{
    HRESULT hr = S_OK;

    IFCPTR(m_Layout);

    IFC(m_Layout->RemoveText(StartPosition, Length));

    if(m_CaretPosition >= StartPosition)
    {
        if(m_CaretPosition <= StartPosition + Length)
        {
            m_CaretPosition = StartPosition;
        }
        else
        {
            m_CaretPosition -= Length;
        }
    }

Cleanup:
    return hr;
}

void 
CTextEditor::OnKeyDown(
	__in CObjectWithType* pSender, 
	__in CRoutedEventArgs* pRoutedEventArgs
	)
{
    HRESULT hr = S_OK;
    CKeyEventArgs* pKeyEventArgs = NULL;
    bool Handled = FALSE;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

    IFC(CastType(pRoutedEventArgs, &pKeyEventArgs));

    if(!pKeyEventArgs->IsHandled())
    {
        switch(pKeyEventArgs->GetKey())
        {
            case Key::Backspace:
                {
                    IFC(HandleBackspace(&Handled));

                    break;
                }

            case Key::Return:
                {
                    IFC(HandleEnter(&Handled));

                    break;
                }
                
            default:
                {
                    break;
                }
        }
    }

    if(Handled)
    {
        pKeyEventArgs->SetHandled();
    }

Cleanup:
    ;
}

void 
CTextEditor::OnKeyUp(
	__in CObjectWithType* pSender,
	__in CRoutedEventArgs* pRoutedEventArgs
	)
{
    HRESULT hr = S_OK;
    CKeyEventArgs* pKeyEventArgs = NULL;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

    IFC(CastType(pRoutedEventArgs, &pKeyEventArgs));

    if(!pKeyEventArgs->IsHandled())
    {
    }

Cleanup:
    ;
}

__checkReturn HRESULT 
CTextEditor::HandleBackspace(
	__out bool* pConsumed
	)
{
    HRESULT hr = S_OK;
    bool Handled = TRUE;

    if(HasSelection())
    {
        IFC(DeleteSelection());
    }
    else
    {
        if(m_CaretPosition > 0)
        {
            IFC(RemoveText(m_CaretPosition - 1, 1));
        }
    }

    if(pConsumed)
    {
        *pConsumed = Handled;
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CTextEditor::HandleEnter(
	__out bool* pConsumed
	)
{
    HRESULT hr = S_OK;
    bool Handled = FALSE;

    if(m_AcceptsEnter)
    {
        IFC(HandleText(L"\n", 1));

        Handled = TRUE;
    }

    if(pConsumed)
    {
        *pConsumed = Handled;
    }

Cleanup:
    return hr;
}

bool 
CTextEditor::HasSelection(
	)
{
    //TODO: Implement selection.
    return FALSE;
}

void 
CTextEditor::SetAcceptsEnter(
	bool AcceptsEnter
	)
{
    m_AcceptsEnter = AcceptsEnter;
}

__checkReturn HRESULT 
CTextEditor::DeleteSelection(
	)
{
    HRESULT hr = S_OK;

    //TODO: Implement selection.

    return hr;
}