#include "StaticPropertyInformation.h"

CStaticPropertyInformation::CStaticPropertyInformation(
    )
{
}

CStaticPropertyInformation::~CStaticPropertyInformation(
    )
{
    for(vector< CStaticProperty* >::iterator It = m_Properties.begin(); It != m_Properties.end(); ++It)
    {
        (*It)->Release();
    }

    m_Properties.clear();
}

__checkReturn HRESULT 
CStaticPropertyInformation::Initialize(
    __in_ecount(PropertyCount) CStaticProperty** ppProperties, 
    UINT32 PropertyCount
    )
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

__override __checkReturn HRESULT 
CStaticPropertyInformation::GetProperty(
    __in_z const WCHAR* pPropertyName, 
    __deref_out_opt CProperty** ppProperty
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pPropertyName);
    IFCPTR(ppProperty);

    for(vector< CStaticProperty* >::iterator It = m_Properties.begin(); It != m_Properties.end(); ++It)
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

__override __checkReturn HRESULT
CStaticPropertyInformation::GetContentProperty(
    __deref_out_opt CProperty** ppProperty
    )
{
    HRESULT hr = S_OK;

    IFCPTR(ppProperty);

    for(vector< CStaticProperty* >::iterator It = m_Properties.begin(); It != m_Properties.end(); ++It)
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

CStaticProperty::CStaticProperty(
    __in_z const WCHAR* pName, 
    const TypeIndex::Value Type, 
    UINT32 Flags, 
    __in_opt GetDefaultPropertyValueFunc DefaultValueFunc,
    __in_opt OnValueChangeFunc ValueChangeFunc
    ) 
    : m_Name(pName)
    , m_Type(Type)
    , m_Flags(Flags)
    , m_DefaultValueFunc(DefaultValueFunc)
    , m_ValueChangeFunc(ValueChangeFunc)
{
}

__override TypeIndex::Value 
CStaticProperty::GetType(
    )
{
    return m_Type;
}

__override __out const WCHAR*
CStaticProperty::GetName(
    )
{
    return m_Name;
}

__override bool 
CStaticProperty::IsCollection(
    )
{
    return (m_Flags & StaticPropertyFlags::Collection) ? TRUE : FALSE;
}

__override bool 
CStaticProperty::IsDictionary(
    )
{
    return (m_Flags & StaticPropertyFlags::Dictionary) ? TRUE : FALSE;
}

__override bool 
CStaticProperty::IsContent(
    )
{
    return (m_Flags & StaticPropertyFlags::Content) ? TRUE : FALSE;
}

__override bool 
CStaticProperty::IsAttached()
{
    return (m_Flags & StaticPropertyFlags::Attached) ? TRUE : FALSE;
}

__override bool 
CStaticProperty::IsReadOnly()
{
    return (m_Flags & StaticPropertyFlags::ReadOnly) ? TRUE : FALSE;
}

__override __checkReturn HRESULT 
CStaticProperty::GetDefaultValue(
    __deref_out_opt CObjectWithType** ppObject
    )
{
    HRESULT hr = S_OK;

    IFCPTR(ppObject);

    IFCPTR(m_DefaultValueFunc);

    IFC(m_DefaultValueFunc(ppObject));

Cleanup:
    return hr;
}

__override __out_opt OnValueChangeFunc
CStaticProperty::GetOnValueChangedCallback(
    )
{
    return m_ValueChangeFunc;
}