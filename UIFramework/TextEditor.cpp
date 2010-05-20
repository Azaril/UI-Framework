#include "TextEditor.h"

CTextEditor::CTextEditor() : m_Host(NULL),
                             m_Layout(NULL)
{
}

CTextEditor::~CTextEditor()
{
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

Cleanup:
    return hr;
}
