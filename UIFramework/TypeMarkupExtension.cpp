#include "TypeMarkupExtension.h"
#include "DelegatingPropertyInformation.h"

//
// Properties
//
CStaticProperty CTypeMarkupExtension::TypeNameProperty( L"TypeName", TypeIndex::String, StaticPropertyFlags::None );

CTypeMarkupExtension::CTypeMarkupExtension() : m_TypeName(NULL),
                                               m_Providers(NULL)
{
}

CTypeMarkupExtension::~CTypeMarkupExtension()
{
    ReleaseObject(m_TypeName);
    ReleaseObject(m_Providers);
}

HRESULT CTypeMarkupExtension::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFCPTR(pProviders);

    m_Providers = pProviders;
    AddRefObject(m_Providers);

Cleanup:
    return hr;
}

HRESULT CTypeMarkupExtension::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &TypeNameProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));

    *ppInformation = pStaticInformation;
    pStaticInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);

    return hr;
}

HRESULT CTypeMarkupExtension::SetValueInternal(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    if(pProperty == &CTypeMarkupExtension::TypeNameProperty)
    {
        IFCEXPECT(pValue->IsTypeOf(TypeIndex::String));

        ReleaseObject(m_TypeName);

        m_TypeName = (CStringValue*)pValue;

        AddRefObject(m_TypeName);
    }
    else
    {
        IFC(CMarkupExtension::SetValueInternal(pProperty, pValue));
    }

Cleanup:
    return hr;
}

HRESULT CTypeMarkupExtension::GetValue(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(pProperty  == &CTypeMarkupExtension::TypeNameProperty)
    {
        *ppValue = m_TypeName;
        AddRefObject(m_TypeName);
    }
    else
    {
        IFC(CMarkupExtension::GetValue(pProperty, ppValue));
    }

Cleanup:
    return hr;
}

HRESULT CTypeMarkupExtension::ExecuteMarkup(CObjectWithType** ppObject)
{
    HRESULT hr = S_OK;
    CClassResolver* pClassResolver = NULL;
    CResolvedClass* pResolvedClass = NULL;
    CTypeValue* pTypeValue = NULL;

    IFCPTR(ppObject);

    IFCPTR(m_TypeName);

    pClassResolver = m_Providers->GetClassResolver();
    IFCPTR(pClassResolver);

    IFC(pClassResolver->ResolveType(m_TypeName->GetValue(), &pResolvedClass));

    IFC(CTypeValue::Create(pResolvedClass->GetType(), &pTypeValue));

    *ppObject = pTypeValue;
    pTypeValue = NULL;

Cleanup:
    ReleaseObject(pResolvedClass);
    ReleaseObject(pTypeValue);

    return hr;
}