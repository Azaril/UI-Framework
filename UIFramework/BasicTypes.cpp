#include "BasicTypes.h"
#include <strsafe.h>

HRESULT CFloatValue::Initialize(FLOAT Value)
{
    m_Value = Value;

    return S_OK;
}

FLOAT CFloatValue::GetValue()
{
    return m_Value;
}

CStringValue::CStringValue() : m_Value(NULL)
{
}

CStringValue::~CStringValue()
{
    delete m_Value;
}

HRESULT CStringValue::Initialize(const WCHAR* pValue)
{
    HRESULT hr = S_OK;
    UINT32 Length = 0;

    IFCPTR(pValue);

    Length = wcslen(pValue);

    IFC(Initialize(pValue, Length));

Cleanup:
    return hr;
}

HRESULT CStringValue::Initialize(const WCHAR* pValue, UINT32 Length)
{
    HRESULT hr = S_OK;
    UINT32 BufferSize = 0;

    IFCPTR(pValue);

    BufferSize = Length + 1;

    m_Value = new WCHAR[BufferSize];
    IFCOOM(m_Value);

    IFC(StringCchCopyN(m_Value, BufferSize, pValue, Length));

Cleanup:
    return hr;
}

const WCHAR* CStringValue::GetValue()
{
    return m_Value;
}