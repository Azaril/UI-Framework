#include "BasicTypes.h"

CStringValue::CStringValue(
    ) 
    : m_Value(NULL)
    , m_Length(0)
{
}

CStringValue::~CStringValue(
    )
{
    delete [] m_Value;
}

__checkReturn HRESULT 
CStringValue::Initialize(
    __in_z const WCHAR* pValue
    )
{
    HRESULT hr = S_OK;
    UINT32 Length = 0;

    IFCPTR(pValue);

    Length = wcslen(pValue);

    IFC(Initialize(pValue, Length));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CStringValue::Initialize(
    __in_ecount(Length) const WCHAR* pValue,
    UINT32 Length
    )
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

__out const WCHAR* 
CStringValue::GetValue(
    )
{
    return m_Value;
}

UINT32 
CStringValue::GetLength(
    )
{
    return m_Length;
}

__override bool 
CStringValue::Equals(
    __in_opt CObjectWithType* pOther
    )
{
    if(pOther != NULL && pOther->GetType() == GetType())
    {
        CStringValue* pTypedOther = (CStringValue*)pOther;

        return (wcscmp(pTypedOther->GetValue(), m_Value) == 0);
    }

    return FALSE;
}

__checkReturn HRESULT 
CStringValue::Clone(
    __deref_out CStringValue** ppClone
    )
{
    HRESULT hr = S_OK;

    IFCPTR(ppClone);

    IFC(CStringValue::Create(m_Value, ppClone));

Cleanup:
    return hr;
}

CDurationValue::CDurationValue(
    ) 
    : m_Forever(FALSE)
    , m_Automatic(FALSE)
{
}

CDurationValue::~CDurationValue(
    )
{
}

__checkReturn HRESULT 
CDurationValue::Initialize(
    const CTimeSpan& Time
    )
{
    HRESULT hr = S_OK;

    m_TimeSpan = Time;

    return hr;
}

__override bool 
CDurationValue::Equals(
    __in_opt CObjectWithType* pOther
    )
{
    if(pOther != NULL && pOther->GetType() == GetType())
    {
        CDurationValue* pTypedOther = (CDurationValue*)pOther;

        return (m_Automatic && pTypedOther->IsAutomatic()) || 
               (m_Forever && pTypedOther->IsForever()) || 
               (m_TimeSpan == *pTypedOther->GetTimeSpan());
    }

    return FALSE;
}

bool 
CDurationValue::IsAutomatic(
    )
{
    return m_Automatic;
}

bool 
CDurationValue::IsForever(
    )
{
    return m_Forever;
}

bool 
CDurationValue::HasTimeSpan(
    )
{
    return (!m_Automatic && !m_Forever);
}

__out_opt const CTimeSpan* 
CDurationValue::GetTimeSpan(
    )
{
    return (!m_Automatic && !m_Forever) ? &m_TimeSpan : NULL;
}

//
// CStringValue
//
extern "C" UIFRAMEWORK_API
TypeIndex::Value 
CStringValue_TypeIndex(
    )
{
    return TypeIndex::String;
}

extern "C" UIFRAMEWORK_API
__out CObjectWithType* 
CStringValue_CastTo_CObjectWithType(
    __in CStringValue* pValue
    )
{
    return pValue;
}

extern "C" UIFRAMEWORK_API
__out_opt CStringValue* 
CObjectWithType_CastTo_CStringValue(
    __in CObjectWithType* pObject
    )
{
    return (pObject->IsTypeOf(TypeIndex::String)) ? (CStringValue*)pObject : NULL;
}

extern "C" UIFRAMEWORK_API
__out const WCHAR*
CStringValue_GetValue(
    __in CStringValue* pValue
    )
{
    return pValue->GetValue();
}

//
// CVisibilityValue
//
extern "C" UIFRAMEWORK_API
TypeIndex::Value
CVisibilityValue_TypeIndex(
    )
{
    return TypeIndex::Visibility;
}

extern "C" UIFRAMEWORK_API
__out CObjectWithType*
CVisibilityValue_CastTo_CObjectWithType(
    __in CVisibilityValue* pValue
    )
{
    return pValue;
}

extern "C" UIFRAMEWORK_API
__out_opt CVisibilityValue* 
CObjectWithType_CastTo_CVisibilityValue(
    __in CObjectWithType* pObject
    )
{
    return (pObject->IsTypeOf(TypeIndex::Visibility)) ? (CVisibilityValue*)pObject : NULL;
}

extern "C" UIFRAMEWORK_API
Visibility::Value 
CVisibilityValue_GetValue(
    __in CVisibilityValue* pValue
    )
{
    return pValue->GetValue();
}

//
// CFloatValue
//
extern "C" UIFRAMEWORK_API
TypeIndex::Value 
CFloatValue_TypeIndex(
    )
{
    return TypeIndex::Float;
}

extern "C" UIFRAMEWORK_API
__out CObjectWithType* 
CFloatValue_CastTo_CObjectWithType(
    __in CFloatValue* pValue
    )
{
    return pValue;
}

extern "C" UIFRAMEWORK_API
__out_opt CFloatValue* 
CObjectWithType_CastTo_CFloatValue(
    __in CObjectWithType* pObject
    )
{
    return (pObject->IsTypeOf(TypeIndex::Float)) ? (CFloatValue*)pObject : NULL;
}

extern "C" UIFRAMEWORK_API
float 
CFloatValue_GetValue(
    __in CFloatValue* pValue
    )
{
    return pValue->GetValue();
}