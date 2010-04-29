#include "StaticPropertyInformation.h"

CStaticPropertyInformation::CStaticPropertyInformation() : m_ClassInformation(NULL)
{
}

CStaticPropertyInformation::~CStaticPropertyInformation()
{
}

HRESULT CStaticPropertyInformation::Initialize(StaticClassProperties *pProperties)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperties);

    m_ClassInformation = pProperties;

Cleanup:
    return hr;
}

HRESULT CStaticPropertyInformation::GetProperty(const WCHAR* pPropertyName, CProperty** ppProperty)
{
    HRESULT hr = S_OK;
    CStaticProperty* pStaticProperty = NULL;

    IFCPTR(pPropertyName);
    IFCPTR(ppProperty);

    for(UINT32 i = 0; i < m_ClassInformation->PropertyCount; i++)
    {
        if(wcscmp(pPropertyName, m_ClassInformation->Properties[i].PropertyName) == 0)
        {
            IFC(CStaticProperty::Create(&m_ClassInformation->Properties[i], &pStaticProperty));

            break;
        }
    }

    IFCPTR(pStaticProperty);

    *ppProperty = pStaticProperty;
    pStaticProperty = NULL;

Cleanup:
    ReleaseObject(pStaticProperty);

    return hr;
}

HRESULT CStaticPropertyInformation::GetContentProperty(CProperty** ppProperty)
{
    HRESULT hr = S_OK;
    CStaticProperty* pStaticProperty = NULL;

    IFCPTR(ppProperty);

    for(UINT32 i = 0; i < m_ClassInformation->PropertyCount; i++)
    {
        if(m_ClassInformation->Properties[i].IsContentProperty)
        {
            IFC(CStaticProperty::Create(&m_ClassInformation->Properties[i], &pStaticProperty));

            break;
        }
    }

    IFCPTR(pStaticProperty);

    *ppProperty = pStaticProperty;
    pStaticProperty = NULL;

Cleanup:
    ReleaseObject(pStaticProperty);

    return hr;
}

CStaticProperty::CStaticProperty() : m_Property(NULL)
{
}

CStaticProperty::~CStaticProperty()
{
}

HRESULT CStaticProperty::Initialize(StaticClassProperty* pProperty)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);

    m_Property = pProperty;

Cleanup:
    return hr;
}

TypeIndex::Value CStaticProperty::GetType()
{
    return m_Property->PropertyType;
}

const WCHAR* CStaticProperty::GetName()
{
    return m_Property->PropertyName;
}