#include "TextEventArgs.h"

CTextEventArgs::CTextEventArgs() : m_Text(NULL)
{
}

CTextEventArgs::~CTextEventArgs()
{
    ReleaseObject(m_Text);
}

HRESULT CTextEventArgs::Initialize(CRoutedEvent* pRoutedEvent, const WCHAR* pText, UINT32 TextLength)
{
    HRESULT hr = S_OK;

    IFC(CInputEventArgs::Initialize(pRoutedEvent));

    IFC(CStringValue::Create(pText, TextLength, &m_Text));

Cleanup:
    return hr;
}

const WCHAR* CTextEventArgs::GetText()
{
    return m_Text->GetValue();
}

UINT32 CTextEventArgs::GetTextLength()
{
    return m_Text->GetLength();
}