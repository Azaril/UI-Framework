#include "StaticPropertyInformation.h"

CStaticPropertyInformation::CStaticPropertyInformation()
{
}

CStaticPropertyInformation::~CStaticPropertyInformation()
{
    for(std::vector< CStaticProperty* >::iterator It = m_Properties.begin(); It != m_Properties.end(); ++It)
    {
        (*It)->Release();
    }

    m_Properties.clear();
}

HRESULT CStaticPropertyInformation::Initialize(CStaticProperty** ppProperties, UINT32 PropertyCount)
{
    HRESULT hr = S_OK;

    if(PropertyCount > 0)
    {
        IFCPTR(ppProperties);
    }

    for(UINT32 i = 0; i < PropertyCount; i++)
    {
        CStaticProperty* pProperty = ppProperties[i];

        IFCPTR(pProperty);

        m_Properties.push_back(pProperty);

        AddRefObject(pProperty);
    }

Cleanup:
    return hr;
}

HRESULT CStaticPropertyInformation::GetProperty(const WCHAR* pPropertyName, CProperty** ppProperty)
{
    HRESULT hr = S_OK;

    IFCPTR(pPropertyName);
    IFCPTR(ppProperty);

    for(std::vector< CStaticProperty* >::iterator It = m_Properties.begin(); It != m_Properties.end(); ++It)
    {
        CStaticProperty* pProperty = (*It);

        if(wcscmp(pPropertyName, pProperty->GetName()) == 0)
        {
            *ppProperty = pProperty;
            AddRefObject(pProperty);

            goto Cleanup;
        }
    }

    hr = E_FAIL;

Cleanup:
    return hr;
}

HRESULT CStaticPropertyInformation::GetContentProperty(CProperty** ppProperty)
{
    HRESULT hr = S_OK;

    IFCPTR(ppProperty);

    for(std::vector< CStaticProperty* >::iterator It = m_Properties.begin(); It != m_Properties.end(); ++It)
    {
        CStaticProperty* pProperty = (*It);

        if(pProperty->IsContent())
        {
            *ppProperty = pProperty;
            AddRefObject(pProperty);

            goto Cleanup;
        }
    }

    hr = E_FAIL;

Cleanup:
    return hr;
}

CStaticProperty::CStaticProperty(const WCHAR* pName, const TypeIndex::Value Type, UINT32 Flags, GetDefaultPropertyValueFunc DefaultValueFunc, OnValueChangeFunc ValueChangeFunc) : m_Name(pName),
                                                                                                                                                                                   m_Type(Type),
                                                                                                                                                                                   m_Flags(Flags),
                                                                                                                                                                                   m_DefaultValueFunc(DefaultValueFunc),
                                                                                                                                                                                   m_ValueChangeFunc(ValueChangeFunc)
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

BOOL CStaticProperty::IsDictionary()
{
    return (m_Flags & StaticPropertyFlags::Dictionary) ? TRUE : FALSE;
}

BOOL CStaticProperty::IsContent()
{
    return (m_Flags & StaticPropertyFlags::Content) ? TRUE : FALSE;
}

BOOL CStaticProperty::IsAttached()
{
    return (m_Flags & StaticPropertyFlags::Attached) ? TRUE : FALSE;
}

BOOL CStaticProperty::IsReadOnly()
{
    return (m_Flags & StaticPropertyFlags::ReadOnly) ? TRUE : FALSE;
}

HRESULT CStaticProperty::GetDefaultValue(CObjectWithType** ppObject)
{
    HRESULT hr = S_OK;

    IFCPTR(ppObject);

    IFCPTR(m_DefaultValueFunc);

    IFC(m_DefaultValueFunc(ppObject));

Cleanup:
    return hr;
}

HRESULT CStaticProperty::OnValueChanged(CPropertyObject* pObjectInstance, CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pObjectInstance);

    if(m_ValueChangeFunc)
    {
        IFC(m_ValueChangeFunc(pObjectInstance, pOldValue, pNewValue));
    }

Cleanup:
    return hr;
}