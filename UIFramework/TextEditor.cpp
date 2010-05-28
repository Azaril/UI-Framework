#include "TextEditor.h"
#include "TextEventArgs.h"

CTextEditor::CTextEditor() : m_Host(NULL),
                             m_Layout(NULL)
{
}

CTextEditor::~CTextEditor()
{
    m_TextConnection.disconnect();

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
        IFC(InsertText(pTextEventArgs->GetText(), pTextEventArgs->GetTextLength()));

        pTextEventArgs->SetHandled(TRUE);
    }

Cleanup:
    ;
}

HRESULT CTextEditor::InsertText(const WCHAR* pText, UINT32 TextLength)
{
    HRESULT hr = S_OK;

    IFCPTR(pText);

    IFC(m_Layout->InsertText(m_Layout->GetEndPosition(), pText, TextLength));

Cleanup:
    return hr;
}