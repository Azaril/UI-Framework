#include "Style.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"
#include "UIElement.h"

//
// Properties
//
namespace StyleProperties
{
    enum Value
    {
        Setters,
        Triggers
    };
}

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( Setters );
DEFINE_GET_DEFAULT_NULL( Triggers );

//
// Properties
//
CStaticProperty CStyle::SettersProperty(TypeIndex::Style, StyleProperties::Setters, L"Setters", TypeIndex::SetterCollection, StaticPropertyFlags::Collection | StaticPropertyFlags::Content, &GET_DEFAULT( Setters ));
CStaticProperty CStyle::TriggersProperty(TypeIndex::Style, StyleProperties::Triggers, L"Triggers", TypeIndex::TriggerCollection, StaticPropertyFlags::Collection, &GET_DEFAULT( Triggers ));

CStyle::CStyle(
    ) 
{
}

CStyle::~CStyle(
    )
{
}

__checkReturn HRESULT 
CStyle::Initialize(
    __in CProviders* pProviders
    )
{
    HRESULT hr = S_OK;

    IFC(CPropertyObject::Initialize(pProviders));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CStyle::ResolveStyle(
    __in CUIElement* pObject,
    __in IStyleCallback* pCallback, 
    __deref_out CResolvedStyle** ppResolvedStyle
    )
{
    HRESULT hr = S_OK;
    CResolvedTriggerActions* pResolvedSetters = NULL;
    CResolvedTriggers* pResolvedTriggers = NULL;

    IFCPTR(pObject);
    IFCPTR(pCallback);

    IFC(ResolveSetters(pObject, pCallback, &pResolvedSetters));

    IFC(pResolvedSetters->Apply());

    IFC(ResolveTriggers(pObject, pCallback, &pResolvedTriggers));

    IFC(CResolvedStyle::Create(pResolvedSetters, pResolvedTriggers, ppResolvedStyle));

Cleanup:
    ReleaseObject(pResolvedSetters);
    ReleaseObject(pResolvedTriggers);

    return hr;
}

__checkReturn HRESULT
CStyle::ResolveSetters(
    __in CUIElement* pObject,
    __in IStyleCallback* pCallback, 
    __deref_out CResolvedTriggerActions** ppResolvedActions
    )
{
    HRESULT hr = S_OK;
    CResolvedTriggerActions* pResolvedSetters = NULL;
    CSetterCollection* pSetters = NULL;

    IFCPTR(pObject);
    IFCPTR(ppResolvedActions);

    IFC(CResolvedTriggerActions::Create(pObject, GetProviders(), pCallback, &pResolvedSetters));

    IFC(GetTypedEffectiveValue(&SettersProperty, &pSetters));

    if (pSetters != NULL)
    {
        for(UINT32 i = 0; i < pSetters->GetCount(); i++)
        {
            CSetter* pSetter = pSetters->GetAtIndex(i);

            IFC(pResolvedSetters->AddAction(pSetter));
        }
    }

    *ppResolvedActions = pResolvedSetters;
    pResolvedSetters = NULL;

Cleanup:
    ReleaseObject(pResolvedSetters);
    ReleaseObject(pSetters);

    return hr;
}

__checkReturn HRESULT 
CStyle::ResolveTriggers(
    __in CUIElement* pObject,
    __in IStyleCallback* pCallback, 
    __deref_out CResolvedTriggers** ppResolvedTriggers
    )
{
    HRESULT hr = S_OK;
    CTriggerCollection* pTriggers = NULL;
    CResolvedTriggers* pResolvedTriggers = NULL;

    IFCPTR(pObject);
    IFCPTR(ppResolvedTriggers);

    IFC(CResolvedTriggers::Create(pObject, GetProviders(), pCallback, &pResolvedTriggers));

    IFC(GetTypedEffectiveValue(&TriggersProperty, &pTriggers));

    if (pTriggers != NULL)
    {
        for(UINT32 i = 0; i < pTriggers->GetCount(); i++)
        {
            CTrigger* pTrigger = pTriggers->GetAtIndex(i);

            IFC(pResolvedTriggers->AddTrigger(pTrigger));
        }
    }

    *ppResolvedTriggers = pResolvedTriggers;
    pResolvedTriggers = NULL;

Cleanup:
    ReleaseObject(pResolvedTriggers);
    ReleaseObject(pTriggers);

    return hr;
}

__checkReturn HRESULT 
CStyle::CreatePropertyInformation(
    __deref_out CPropertyInformation** ppInformation
    )
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &SettersProperty,
        &TriggersProperty
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
CStyle::GetLayeredValue(
    __in CProperty* pProperty,
    __deref_out CLayeredValue** ppLayeredValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    if (pProperty->GetOwningType() == TypeIndex::Style)
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        switch(pStaticProperty->GetLocalIndex())
        {
            case StyleProperties::Setters:
                {
                    *ppLayeredValue = &m_Setters;
                    break;
                }

            case StyleProperties::Triggers:
                {
                    *ppLayeredValue = &m_Triggers;
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

CResolvedStyle::CResolvedStyle(
    ) 
    : m_Actions(NULL)
    , m_Triggers(NULL)
{
}

CResolvedStyle::~CResolvedStyle(
    )
{
    ReleaseObject(m_Actions);
    ReleaseObject(m_Triggers);
}

__checkReturn HRESULT
CResolvedStyle::Initialize(
    __in CResolvedTriggerActions* pSetters,
    __in CResolvedTriggers* pTriggers
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pSetters);
    IFCPTR(pTriggers);

    m_Actions = pSetters;
    AddRefObject(m_Actions);

    m_Triggers = pTriggers;
    AddRefObject(m_Triggers);

Cleanup:
    return hr;
}