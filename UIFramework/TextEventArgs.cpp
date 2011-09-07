#include "TextEventArgs.h"

CTextEventArgs::CTextEventArgs(
    ) 
    : m_Text(NULL)
{
}

CTextEventArgs::~CTextEventArgs(
    )
{
    ReleaseObject(m_Text);
}

__checkReturn HRESULT 
CTextEventArgs::Initialize(
    __in CRoutedEvent* pRoutedEvent, 
    __in_ecount(TextLength) const WCHAR* pText,
    UINT32 TextLength
    )
{
    HRESULT hr = S_OK;

    IFC(CInputEventArgs::Initialize(pRoutedEvent));

    IFC(CStringValue::Create(pText, TextLength, &m_Text));

Cleanup:
    return hr;
}

__out const WCHAR*
CTextEventArgs::GetText(
    )
{
    return m_Text->GetValue();
}

UINT32 
CTextEventArgs::GetTextLength(
    )
{
    return m_Text->GetLength();
}

//
// CTextEventArgs
//
extern "C" __declspec(dllexport)
TypeIndex::Value
CTextEventArgs_TypeIndex(
    )
{
    return TypeIndex::TextEventArgs;
}

extern "C" __declspec(dllexport)
__out CInputEventArgs* 
CTextEventArgs_CastTo_CInputEventArgs(
    __in CTextEventArgs* pArgs
    )
{
    return pArgs;
}

extern "C" __declspec(dllexport)
__out_opt CTextEventArgs* 
CObjectWithType_CastTo_CTextEventArgs(
    __in CObjectWithType* pObject
    )
{
    return (pObject->IsTypeOf(TypeIndex::TextEventArgs)) ? (CTextEventArgs*)pObject : NULL;
}