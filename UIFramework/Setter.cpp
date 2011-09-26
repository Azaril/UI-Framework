#include "Setter.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"
#include "UIElement.h"

//
// Properties
//
CStaticProperty CSetter::PropertyProperty(L"Property", TypeIndex::String, StaticPropertyFlags::None);
CStaticProperty CSetter::ValueProperty(L"Value", TypeIndex::ParserCommandList, StaticPropertyFlags::Content);

CSetter::CSetter(
    ) 
    : m_Property(NULL)
    , m_Value(NULL)
    , m_Providers(NULL)
    , m_CachedValue(NULL)
{
}

CSetter::~CSetter(
    )
{
    ReleaseObject(m_Property);
    ReleaseObject(m_Value);
    ReleaseObject(m_CachedValue);
    ReleaseObject(m_Providers);
}

__checkReturn HRESULT
CSetter::Initialize(
    __in CProviders* pProviders
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProviders);

    m_Providers = pProviders;
    AddRefObject(m_Providers);

Cleanup:
    return hr;
}

__out_opt const WCHAR* 
CSetter::GetPropertyName(
    )
{
    return (m_Property != NULL) ? m_Property->GetValue() : NULL;
}

__out_opt CObjectWithType* 
CSetter::GetPropertyValue(
    )
{
    return m_Value;
}

__checkReturn HRESULT 
CSetter::SetPropertyInternal(
    __in_z const WCHAR* pProperty
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);

    IFC(CStringValue::Create(pProperty, &m_Property));

Cleanup:
    return hr;
}

__checkReturn HRESULT
CSetter::SetPropertyValueInternal(
    __in CParserCommandList* pValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pValue);

    ReleaseObject(m_Value);
    ReleaseObject(m_CachedValue);

    m_Value = pValue;

    AddRefObject(m_Value);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CSetter::ResolveAction(
    __in CUIElement* pObject, 
    __in IStyleCallback* pCallback, 
    __deref_out CResolvedTriggerAction** ppResolvedAction
    )
{
    HRESULT hr = S_OK;
    CClassResolver* pClassResolver = NULL;
    CProperty* pProperty = NULL;
    CObjectWithType* pValue = NULL;
    CResolvedSetter* pResolvedSetter = NULL;

    IFCPTR(pObject);
    IFCPTR(ppResolvedAction);

    pClassResolver = m_Providers->GetClassResolver();
    IFCPTR(pClassResolver);

    IFC(pClassResolver->ResolveProperty(GetPropertyName(), pObject->GetType(), &pProperty));

    if(m_CachedValue == NULL)
    {
        IFC(m_Value->Execute(NULL, &pValue));

        if(pValue->IsShareable())
        {
            m_CachedValue = pValue;
            AddRefObject(m_CachedValue);
        }
    }
    else
    {
        pValue = m_CachedValue;
        AddRefObject(pValue);
    }

    IFC(CResolvedSetter::Create(pProperty, pValue, pCallback, &pResolvedSetter));

    *ppResolvedAction = pResolvedSetter;
    pResolvedSetter = NULL;

Cleanup:
    ReleaseObject(pProperty);
    ReleaseObject(pValue);
    ReleaseObject(pResolvedSetter);

    return hr;
}

__checkReturn HRESULT 
CSetter::CreatePropertyInformation(
    __deref_out CPropertyInformation** ppInformation
    )
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &PropertyProperty,
        &ValueProperty
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
CSetter::SetValueInternal(
    __in CProperty* pProperty, 
    __in CObjectWithType* pValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    if(pProperty == &CSetter::PropertyProperty)
    {
        IFCEXPECT(pValue->IsTypeOf(TypeIndex::String));

        CStringValue* pStringValue = (CStringValue*)pValue;

        IFC(SetPropertyInternal(pStringValue->GetValue()));
    }
    else if(pProperty == &CSetter::ValueProperty)
    {
        IFCEXPECT(pValue->IsTypeOf(TypeIndex::ParserCommandList));

        CParserCommandList* pCommandList = (CParserCommandList*)pValue;

        IFC(SetPropertyValueInternal(pCommandList));
    }
    else
    {
        IFC(CPropertyObject::SetValueInternal(pProperty, pValue));
    }

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CSetter::GetValueInternal(
    __in CProperty* pProperty, 
    __deref_out_opt CObjectWithType** ppValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(pProperty == &CSetter::PropertyProperty)
    {
        *ppValue = m_Property;
        AddRefObject(m_Property);
    }
    else if(pProperty == &CSetter::ValueProperty)
    {
        *ppValue = m_Value;
        AddRefObject(m_Value);
    }
    else
    {
        IFC(CPropertyObject::GetValueInternal(pProperty, ppValue));
    }

Cleanup:
    return hr;
}

CResolvedSetter::CResolvedSetter(
    ) 
    : m_Property(NULL)
    , m_Value(NULL)
    , m_Callback(NULL)
{
}

CResolvedSetter::~CResolvedSetter(
    )
{
    ReleaseObject(m_Property);
    ReleaseObject(m_Value);
}

__checkReturn HRESULT 
CResolvedSetter::Initialize(
    __in CProperty* pProperty, 
    __in CObjectWithType* pValue,
    __in IStyleCallback* pCallback
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);
    IFCPTR(pCallback);

    m_Property = pProperty;
    AddRefObject(m_Property);

    m_Value = pValue;
    AddRefObject(m_Value);

    m_Callback = pCallback;

Cleanup:
    return hr;
}

__checkReturn HRESULT
CResolvedSetter::Apply(
    )
{
    HRESULT hr = S_OK;

    IFC(m_Callback->SetStyleValue(m_Property, m_Value));

Cleanup:
    return hr;
}