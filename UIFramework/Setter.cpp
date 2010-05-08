#include "Setter.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"

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

HRESULT CSetter::SetValue(CProperty* pProperty, CObjectWithType* pValue)
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
        IFC(CPropertyObject::SetValue(pProperty, pValue));
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