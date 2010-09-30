#include "BasicTypes.h"

CStringValue::CStringValue() : m_Value(NULL),
                               m_Length(0)
{
}

CStringValue::~CStringValue()
{
    delete [] m_Value;
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

    m_Length = Length;

    BufferSize = m_Length + 1;

    m_Value = new WCHAR[BufferSize];
    IFCOOM(m_Value);

    wcsncpy(m_Value, pValue, Length);
    m_Value[m_Length] = '\0';

Cleanup:
    return hr;
}

const WCHAR* CStringValue::GetValue()
{
    return m_Value;
}

UINT32 CStringValue::GetLength()
{
    return m_Length;
}

BOOL CStringValue::Equals(CObjectWithType* pOther)
{
    if(pOther && pOther->GetType() == GetType())
    {
        CStringValue* pTypedOther = (CStringValue*)pOther;

        return (wcscmp(pTypedOther->GetValue(), m_Value) == 0);
    }

    return FALSE;
}

HRESULT CStringValue::Clone(CStringValue** ppClone)
{
    HRESULT hr = S_OK;

    IFCPTR(ppClone);

    IFC(CStringValue::Create(m_Value, ppClone));

Cleanup:
    return hr;
}

//
// CStringValue
//
extern "C" __declspec(dllexport)
TypeIndex::Value CStringValue_TypeIndex()
{
    return TypeIndex::String;
}

extern "C" __declspec(dllexport)
CObjectWithType* CStringValue_CastTo_CObjectWithType(CStringValue* pValue)
{
    return pValue;
}

extern "C" __declspec(dllexport)
CStringValue* CObjectWithType_CastTo_CStringValue(CObjectWithType* pObject)
{
    return (pObject->IsTypeOf(TypeIndex::String)) ? (CStringValue*)pObject : NULL;
}

extern "C" __declspec(dllexport)
const WCHAR* CStringValue_GetValue(CStringValue* pValue)
{
    return pValue->GetValue();
}

//
// CVisibilityValue
//
extern "C" __declspec(dllexport)
TypeIndex::Value CVisibilityValue_TypeIndex()
{
    return TypeIndex::Visibility;
}

extern "C" __declspec(dllexport)
CObjectWithType* CVisibilityValue_CastTo_CObjectWithType(CVisibilityValue* pValue)
{
    return pValue;
}

extern "C" __declspec(dllexport)
CVisibilityValue* CObjectWithType_CastTo_CVisibilityValue(CObjectWithType* pObject)
{
    return (pObject->IsTypeOf(TypeIndex::Visibility)) ? (CVisibilityValue*)pObject : NULL;
}

extern "C" __declspec(dllexport)
Visibility::Value CVisibilityValue_GetValue(CVisibilityValue* pValue)
{
    return pValue->GetValue();
}