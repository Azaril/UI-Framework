#include "TextEditor.h"
#include "TextEventArgs.h"
#include "KeyboardEventArgs.h"

CTextEditor::CTextEditor() : m_Host(NULL),
                             m_Layout(NULL),
                             m_CaretPosition(0)
{
}

CTextEditor::~CTextEditor()
{
    m_TextConnection.disconnect();
    m_KeyDownConnection.disconnect();
    m_KeyUpConnection.disconnect();

    ReleaseObject(m_Layout);
}

HRESULT CTextEditor::Initialize(CUIElement* pElement, CEditableTextLayout* pLayout)
{
    HRESULT hr = S_OK;

    IFCPTR(pElement);
    IFCPTR(pLayout);

    m_Host = pElement;

    m_Layout = pLayout;
    AddRefObject(m_Layout);

    IFC(m_Host->AddHandler(&CUIElement::TextEvent, bind(&CTextEditor::OnText, this, _1, _2), &m_TextConnection));
    IFC(m_Host->AddHandler(&CUIElement::KeyDownEvent, bind(&CTextEditor::OnKeyDown, this, _1, _2), &m_KeyDownConnection));
    IFC(m_Host->AddHandler(&CUIElement::KeyUpEvent, bind(&CTextEditor::OnKeyUp, this, _1, _2), &m_KeyUpConnection));

Cleanup:
    return hr;
}

HRESULT CTextEditor::SetText(const WCHAR* pText, UINT32 TextLength)
{
    HRESULT hr = S_OK;

    IFC(m_Layout->SetText(pText, TextLength));

    m_CaretPosition = TextLength;

Cleanup:
    return hr;
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

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

    IFC(CastType(pRoutedEventArgs, &pKeyEventArgs));

    if(!pKeyEventArgs->IsHandled())
    {
        switch(pKeyEventArgs->GetKey())
        {
            //TODO: Switch to key enumeration.
            case VK_BACK:
                {
                    IFC(HandleBackspace());

                    pKeyEventArgs->SetHandled();

                    break;
                }
        }
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

HRESULT CTextEditor::HandleBackspace()
{
    HRESULT hr = S_OK;

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

Cleanup:
    return hr;
}

BOOL CTextEditor::HasSelection()
{
    //TODO: Implement selection.
    return FALSE;
}

HRESULT CTextEditor::DeleteSelection()
{
    HRESULT hr = S_OK;

    //TODO: Implement selection.

    return hr;
}