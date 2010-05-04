#include "StaticPropertyInformation.h"

CStaticPropertyInformation::CStaticPropertyInformation() : m_Properties(NULL),
                                                           m_PropertyCount(0)
{
}

CStaticPropertyInformation::~CStaticPropertyInformation()
{
}

HRESULT CStaticPropertyInformation::Initialize(CStaticProperty* pProperties, UINT32 PropertyCount)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperties);

    m_Properties = pProperties;
    m_PropertyCount = PropertyCount;

Cleanup:
    return hr;
}

HRESULT CStaticPropertyInformation::GetProperty(const WCHAR* pPropertyName, CProperty** ppProperty)
{
    HRESULT hr = S_OK;

    IFCPTR(pPropertyName);
    IFCPTR(ppProperty);

    for(UINT32 i = 0; i < m_PropertyCount; i++)
    {
        if(wcscmp(pPropertyName, m_Properties[i].GetName()) == 0)
        {
            *ppProperty = &m_Properties[i];

            goto Cleanup;
        }
    }

    IFC(E_FAIL);

Cleanup:
    return hr;
}

HRESULT CStaticPropertyInformation::GetContentProperty(CProperty** ppProperty)
{
    HRESULT hr = S_OK;

    IFCPTR(ppProperty);

    for(UINT32 i = 0; i < m_PropertyCount; i++)
    {
        if(m_Properties[i].IsContent())
        {
            *ppProperty = &m_Properties[i];

            goto Cleanup;
        }
    }

    IFC(E_FAIL);

Cleanup:
    return hr;
}

CStaticProperty::CStaticProperty(const WCHAR* pName, const TypeIndex::Value Type, UINT32 Flags) : m_Name(pName),
                                                                                                  m_Type(Type),
                                                                                                  m_Flags(Flags)
{
}

INT32 CStaticProperty::AddRef()
{
    return 1;
}

INT32 CStaticProperty::Release()
{
    return 1;
}

TypeIndex::Value CStaticProperty::GetType()
{
    return m_Type;
}

const WCHAR* CStaticProperty::GetName()
{
    return m_Name;
}

BOOL CStaticProperty::IsCollection()
{
    return (m_Flags & StaticPropertyFlags::Collection) ? TRUE : FALSE;
}

BOOL CStaticProperty::IsContent()
{
    return (m_Flags & StaticPropertyFlags::Content) ? TRUE : FALSE;
}

BOOL CStaticProperty::IsAttached()
{
    return (m_Flags & StaticPropertyFlags::Attached) ? TRUE : FALSE;
}