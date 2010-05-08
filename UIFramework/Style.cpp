#include "Style.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"

//
// Properties
//
CStaticProperty CStyle::SettersProperty( L"Setters", TypeIndex::Setter, StaticPropertyFlags::Collection );

CStyle::CStyle() : m_Setters(NULL)
{
}

CStyle::~CStyle()
{
    ReleaseObject(m_Setters);
}

HRESULT CStyle::Initialize()
{
    HRESULT hr = S_OK;

    IFC(CSetterCollection::Create(&m_Setters));

Cleanup:
    return hr;
}

HRESULT CStyle::ResolveSetters(CPropertyObject* pObject, CProviders* pProviders, IResolvedStyleSetterCallback* pCallback)
{
    HRESULT hr = S_OK;
    CClassResolver* pClassResolver = NULL;
    CPropertyInformation* pProperties = NULL;
    CProperty* pProperty = NULL;

    IFCPTR(pObject);
    IFCPTR(pProviders);
    IFCPTR(pCallback);

    pClassResolver = pProviders->GetClassResolver();
    IFCPTR(pClassResolver);

    IFC(pClassResolver->ResolveProperties(pObject->GetType(), &pProperties));

    for(UINT32 i = 0; i < m_Setters->GetCount(); i++)
    {
        CSetter* pSetter = m_Setters->GetAtIndex(i);

        const WCHAR* pName = pSetter->GetPropertyName();
        IFCPTR(pName);

        CObjectWithType* pValue = pSetter->GetPropertyValue();
        IFCPTR(pValue);

        IFC(pClassResolver->ResolveProperty(pName, pProperties, &pProperty));

        IFC(pCallback->OnResolvedSetter(pProperty, pValue));

        ReleaseObject(pProperty);
    }

Cleanup:
    ReleaseObject(pProperties);
    ReleaseObject(pProperty);

    return hr;
}

HRESULT CStyle::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;

    IFCPTR(ppInformation);

    CStaticProperty* Properties[] = 
    {
        &SettersProperty
    };

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));

    *ppInformation = pStaticInformation;
    pStaticInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);

    return hr;
}

HRESULT CStyle::SetValueInternal(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    if(pProperty == &CStyle::SettersProperty)
    {
        IFC(E_NOTIMPL);
    }
    else
    {
        IFC(CPropertyObject::SetValueInternal(pProperty, pValue));
    }

Cleanup:
    return hr;
}

HRESULT CStyle::GetValue(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(pProperty == &CStyle::SettersProperty)
    {
        *ppValue = m_Setters;
        AddRefObject(m_Setters);
    }
    else
    {
        IFC(CPropertyObject::GetValue(pProperty, ppValue));
    }

Cleanup:
    return hr;
}