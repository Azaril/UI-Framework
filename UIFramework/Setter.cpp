#include "Setter.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"
#include "UIElement.h"

//
// Properties
//
namespace SetterProperties
{
    enum Value
    {
        Property,
        Value
    };
}

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( Value );
DEFINE_INSTANCE_CHANGE_CALLBACK( CSetter, OnValueChanged );

//
// Properties
//
CStaticProperty CSetter::PropertyProperty(TypeIndex::Setter, SetterProperties::Property, L"Property", TypeIndex::String, StaticPropertyFlags::None);
CStaticProperty CSetter::ValueProperty(TypeIndex::Setter, SetterProperties::Value, L"Value", TypeIndex::ParserCommandList, StaticPropertyFlags::Content, &GET_DEFAULT( Value ), &INSTANCE_CHANGE_CALLBACK( CSetter, OnValueChanged ));

CSetter::CSetter(
    ) 
    : m_Providers(NULL)
    , m_CachedValue(NULL)
{
}

CSetter::~CSetter(
    )
{
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
    CStringValue* pPropertyAsString = NULL;
    CParserCommandList* pCurrentValue = NULL;

    IFCPTR(pObject);
    IFCPTR(ppResolvedAction);

    pClassResolver = m_Providers->GetClassResolver();
    IFCPTR(pClassResolver);

    IFC(GetTypedEffectiveValue(&PropertyProperty, &pPropertyAsString));

    IFC(pClassResolver->ResolveProperty(pPropertyAsString->GetValue(), pObject->GetType(), &pProperty));

    if(m_CachedValue == NULL)
    {
        IFC(GetTypedEffectiveValue(&ValueProperty, &pCurrentValue));

        IFCPTR(pCurrentValue);

        IFC(pCurrentValue->Execute(NULL, &pValue));

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
    ReleaseObject(pPropertyAsString);
    ReleaseObject(pCurrentValue);

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
CSetter::GetLayeredValue(
    __in CProperty* pProperty,
    __deref_out CLayeredValue** ppLayeredValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    if (pProperty->GetOwningType() == TypeIndex::Setter)
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        switch(pStaticProperty->GetLocalIndex())
        {
            case SetterProperties::Property:
                {
                    *ppLayeredValue = &m_Property;
                    break;
                }

            case SetterProperties::Value:
                {
                    *ppLayeredValue = &m_Value;
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
        IFC_NOTRACE(CPropertyObject::GetLayeredValue(pProperty, ppLayeredValue));
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT
CSetter::OnValueChanged(
    __in_opt CObjectWithType* pOldValue, 
    __in_opt CObjectWithType* pNewValue
    )
{
    HRESULT hr = S_OK;

    ReleaseObject(m_CachedValue);
    
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