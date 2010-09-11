#include "TextEditor.h"
#include "TextEventArgs.h"
#include "KeyboardEventArgs.h"

CTextEditor::CTextEditor() : m_Host(NULL),
                             m_Layout(NULL),
                             m_CaretPosition(0),
                             m_AcceptsEnter(TRUE)
{
}

CTextEditor::~CTextEditor()
{
    SetTextHost(NULL);
    SetTextLayout(NULL);
}

HRESULT CTextEditor::Initialize()
{
    HRESULT hr = S_OK;

    return hr;
}

HRESULT CTextEditor::SetTextHost(CUIElement* pElement)
{
    HRESULT hr = S_OK;

    if(m_Host)
    {
        m_TextConnection.disconnect();
        m_KeyDownConnection.disconnect();
        m_KeyUpConnection.disconnect();

        ReleaseObject(m_Host);
    }

    m_Host = pElement;
    
    if(m_Host)
    {
        AddRefObject(m_Host);

        IFC(m_Host->AddHandler(&CUIElement::TextEvent, bind(&CTextEditor::OnText, this, _1, _2), &m_TextConnection));
        IFC(m_Host->AddHandler(&CUIElement::KeyDownEvent, bind(&CTextEditor::OnKeyDown, this, _1, _2), &m_KeyDownConnection));
        IFC(m_Host->AddHandler(&CUIElement::KeyUpEvent, bind(&CTextEditor::OnKeyUp, this, _1, _2), &m_KeyUpConnection));
    }

Cleanup:
    return hr;
}

HRESULT CTextEditor::SetTextLayout(CEditableTextLayout* pLayout)
{
    HRESULT hr = S_OK;

    if(m_Layout)
    {
        m_TextHolder.assign(m_Layout->GetText());

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

HRESULT CTextEditor::SetText(const WCHAR* pText, UINT32 TextLength)
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

const WCHAR* CTextEditor::GetText()
{
    if(m_Layout)
    {
        return m_Layout->GetText();
    }

    return NULL;
}

void CTextEditor::OnText(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs)
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

HRESULT CTextEditor::HandleText(const WCHAR* pText, UINT32 TextLength)
{
    HRESULT hr = S_OK;

    IFCPTR(pText);

    IFC(InsertText(m_CaretPosition, pText, TextLength));

Cleanup:
    return hr;
}

HRESULT CTextEditor::InsertText(UINT32 StartPosition, const WCHAR* pText, UINT32 TextLength)
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

HRESULT CTextEditor::RemoveText(UINT32 StartPosition, UINT32 Length)
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

void CTextEditor::OnKeyDown(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;
    CKeyEventArgs* pKeyEventArgs = NULL;
    BOOL Handled = FALSE;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

    IFC(CastType(pRoutedEventArgs, &pKeyEventArgs));

    if(!pKeyEventArgs->IsHandled())
    {
        switch(pKeyEventArgs->GetKey())
        {
            //TODO: Switch to key enumeration.
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
        }
    }

    if(Handled)
    {
        pKeyEventArgs->SetHandled();
    }

Cleanup:
    ;
}

void CTextEditor::OnKeyUp(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs)
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

HRESULT CTextEditor::HandleBackspace(BOOL* pConsumed)
{
    HRESULT hr = S_OK;
    BOOL Handled = TRUE;

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

HRESULT CTextEditor::HandleEnter(BOOL* pConsumed)
{
    HRESULT hr = S_OK;
    BOOL Handled = FALSE;

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

BOOL CTextEditor::HasSelection()
{
    //TODO: Implement selection.
    return FALSE;
}

void CTextEditor::SetAcceptsEnter(BOOL AcceptsEnter)
{
    m_AcceptsEnter = AcceptsEnter;
}

HRESULT CTextEditor::DeleteSelection()
{
    HRESULT hr = S_OK;

    //TODO: Implement selection.

    return hr;
}