#include "Setter.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"
#include "UIElement.h"

//
// Properties
//
CStaticProperty CSetter::PropertyProperty( L"Property", TypeIndex::String, StaticPropertyFlags::None );
CStaticProperty CSetter::ValueProperty( L"Value", TypeIndex::Object, StaticPropertyFlags::Content );

CSetter::CSetter() : m_Property(NULL),
                     m_Value(NULL)
{
}

CSetter::~CSetter()
{
    ReleaseObject(m_Property);
    ReleaseObject(m_Value);
}

HRESULT CSetter::Initialize()
{
    HRESULT hr = S_OK;

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

HRESULT CSetter::SetPropertyValueInternal(CObjectWithType* pValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pValue);

    ReleaseObject(m_Value);

    m_Value = pValue;

    AddRefObject(m_Value);

Cleanup:
    return hr;
}

HRESULT CSetter::ResolveSetter(CUIElement* pObject, CProviders* pProviders, IStyleCallback* pCallback, CResolvedSetter** ppResolvedSetter)
{
    HRESULT hr = S_OK;
    CClassResolver* pClassResolver = NULL;
    CProperty* pProperty = NULL;

    IFCPTR(pObject);
    IFCPTR(pProviders);
    IFCPTR(ppResolvedSetter);

    pClassResolver = pProviders->GetClassResolver();
    IFCPTR(pClassResolver);

    IFC(pClassResolver->ResolveProperty(GetPropertyName(), pObject->GetType(), &pProperty));

    IFC(CResolvedSetter::Create(pProperty, m_Value, pCallback, ppResolvedSetter));

Cleanup:
    ReleaseObject(pProperty);

    return hr;
}

HRESULT CSetter::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;

    IFCPTR(ppInformation);

    CStaticProperty* Properties[] = 
    {
        &PropertyProperty,
        &ValueProperty
    };

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
        IFC(SetPropertyValueInternal(pValue));
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
        IFC(E_NOTIMPL);
    }
    else if(pProperty == &CSetter::ValueProperty)
    {
        IFC(E_NOTIMPL);
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