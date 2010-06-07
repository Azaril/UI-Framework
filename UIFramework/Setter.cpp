#include "Setter.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"
#include "UIElement.h"

//
// Properties
//
CStaticProperty CSetter::PropertyProperty( L"Property", TypeIndex::String, StaticPropertyFlags::None );
CStaticProperty CSetter::ValueProperty( L"Value", TypeIndex::ParserCommandList, StaticPropertyFlags::Content );

CSetter::CSetter() : m_Property(NULL),
                     m_Value(NULL),
                     m_Providers(NULL),
                     m_CachedValue(NULL)
{
}

CSetter::~CSetter()
{
    ReleaseObject(m_Property);
    ReleaseObject(m_Value);
    ReleaseObject(m_CachedValue);
    ReleaseObject(m_Providers);
}

HRESULT CSetter::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFCPTR(pProviders);

    m_Providers = pProviders;
    AddRefObject(m_Providers);

Cleanup:
    return hr;
}

const WCHAR* CSetter::GetPropertyName()
{
    return (m_Property != NULL) ? m_Property->GetValue() : NULL;
}

CObjectWithType* CSetter::GetPropertyValue()
{
    return m_Value;
}

HRESULT CSetter::SetPropertyInternal(const WCHAR* pProperty)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);

    IFC(CStringValue::Create(pProperty, &m_Property));

Cleanup:
    return hr;
}

HRESULT CSetter::SetPropertyValueInternal(CParserCommandList* pValue)
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

HRESULT CSetter::ResolveSetter(CUIElement* pObject, IStyleCallback* pCallback, CResolvedSetter** ppResolvedSetter)
{
    HRESULT hr = S_OK;
    CClassResolver* pClassResolver = NULL;
    CProperty* pProperty = NULL;
    CObjectWithType* pValue = NULL;

    IFCPTR(pObject);
    IFCPTR(ppResolvedSetter);

    pClassResolver = m_Providers->GetClassResolver();
    IFCPTR(pClassResolver);

    IFC(pClassResolver->ResolveProperty(GetPropertyName(), pObject->GetType(), &pProperty));

    if(m_CachedValue == NULL)
    {
        IFC(m_Value->Execute(&pValue));

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

    IFC(CResolvedSetter::Create(pProperty, pValue, pCallback, ppResolvedSetter));

Cleanup:
    ReleaseObject(pProperty);
    ReleaseObject(pValue);

    return hr;
}

HRESULT CSetter::CreatePropertyInformation(CPropertyInformation **ppInformation)
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

HRESULT CSetter::SetValueInternal(CProperty* pProperty, CObjectWithType* pValue)
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

HRESULT CSetter::GetValue(CProperty* pProperty, CObjectWithType** ppValue)
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
        IFC(CPropertyObject::GetValue(pProperty, ppValue));
    }

Cleanup:
    return hr;
}




CResolvedSetter::CResolvedSetter() : m_Property(NULL),
                                     m_Value(NULL),
                                     m_Callback(NULL)
{
}

CResolvedSetter::~CResolvedSetter()
{
    ReleaseObject(m_Property);
    ReleaseObject(m_Value);
}

HRESULT CResolvedSetter::Initialize(CProperty* pProperty, CObjectWithType* pValue, IStyleCallback* pCallback)
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

HRESULT CResolvedSetter::Apply()
{
    HRESULT hr = S_OK;

    IFC(m_Callback->SetStyleValue(m_Property, m_Value));

Cleanup:
    return hr;
}