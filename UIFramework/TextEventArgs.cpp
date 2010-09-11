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

//
// CTextEventArgs
//
extern "C" __declspec(dllexport)
TypeIndex::Value CTextEventArgs_TypeIndex()
{
    return TypeIndex::TextEventArgs;
}

extern "C" __declspec(dllexport)
CInputEventArgs* CTextEventArgs_CastTo_CInputEventArgs(CTextEventArgs* pArgs)
{
    return pArgs;
}

extern "C" __declspec(dllexport)
CTextEventArgs* CObjectWithType_CastTo_CTextEventArgs(CObjectWithType* pObject)
{
    return (pObject->IsTypeOf(TypeIndex::TextEventArgs)) ? (CTextEventArgs*)pObject : NULL;
}