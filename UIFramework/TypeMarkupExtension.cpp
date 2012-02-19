#include "TypeMarkupExtension.h"
#include "DelegatingPropertyInformation.h"

//
// Properties
//
namespace TypeMarkupExtensionProperties
{
    enum Value
    {
        TypeName
    };
}

//
// Properties
//
CStaticProperty CTypeMarkupExtension::TypeNameProperty(TypeIndex::TypeMarkupExtension, TypeMarkupExtensionProperties::TypeName, L"TypeName", TypeIndex::String, StaticPropertyFlags::None);

CTypeMarkupExtension::CTypeMarkupExtension(
	) 
{
}

CTypeMarkupExtension::~CTypeMarkupExtension(
	)
{
}

__checkReturn HRESULT 
CTypeMarkupExtension::Initialize(
	__in CProviders* pProviders
	)
{
    HRESULT hr = S_OK;

    IFC(CMarkupExtension::Initialize(pProviders));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CTypeMarkupExtension::CreatePropertyInformation(
	__deref_out CPropertyInformation **ppInformation
	)
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

__override __checkReturn HRESULT 
CTypeMarkupExtension::GetLayeredValue(
    __in CProperty* pProperty,
    __deref_out CLayeredValue** ppLayeredValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    if (pProperty->GetOwningType() == TypeIndex::TypeMarkupExtension)
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        switch(pStaticProperty->GetLocalIndex())
        {
            case TypeMarkupExtensionProperties::TypeName:
                {
                    *ppLayeredValue = &m_TypeName;
                    break;
                }

            default:
                {
                    IFC(E_UNEXPECTED);
                }
        }
    }
    else
    {
        IFC_NOTRACE(CMarkupExtension::GetLayeredValue(pProperty, ppLayeredValue));
    }

Cleanup:
    return hr;
}

__override __checkReturn HRESULT
CTypeMarkupExtension::ExecuteMarkup(
	__deref_out CObjectWithType** ppObject
	)
{
    HRESULT hr = S_OK;
    CClassResolver* pClassResolver = NULL;
    CResolvedClass* pResolvedClass = NULL;
    CTypeValue* pTypeValue = NULL;
    CStringValue* pTypeName = NULL;

    IFCPTR(ppObject);

    IFC(GetTypedEffectiveValue(&TypeNameProperty, &pTypeName));
    IFCPTR(pTypeName);

    pClassResolver = GetProviders()->GetClassResolver();
    IFCPTR(pClassResolver);

    IFC(pClassResolver->ResolveType(pTypeName->GetValue(), &pResolvedClass));

    IFC(CTypeValue::Create(pResolvedClass->GetType(), &pTypeValue));

    *ppObject = pTypeValue;
    pTypeValue = NULL;

Cleanup:
    ReleaseObject(pResolvedClass);
    ReleaseObject(pTypeValue);
    ReleaseObject(pTypeName);

    return hr;
}