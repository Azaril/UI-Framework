#include "EventTrigger.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"
#include "DelegatingPropertyInformation.h"
#include "UIElement.h"

//
// Properties
//
namespace EventTriggerProperties
{
    enum Value
    {
        RoutedEvent,
        Actions
    };
}

//
// Properties
//
CStaticProperty CEventTrigger::RoutedEventProperty(TypeIndex::EventTrigger, EventTriggerProperties::RoutedEvent, L"RoutedEvent", TypeIndex::RoutedEvent, StaticPropertyFlags::None);
CStaticProperty CEventTrigger::ActionsProperty(TypeIndex::EventTrigger, EventTriggerProperties::Actions, L"Actions", TypeIndex::TriggerActionCollection, StaticPropertyFlags::Collection | StaticPropertyFlags::Content);

CEventTrigger::CEventTrigger(
    ) 
    : m_Providers(NULL)
{
}

CEventTrigger::~CEventTrigger(
    )
{
    ReleaseObject(m_Providers);
}

__checkReturn HRESULT 
CEventTrigger::Initialize(
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

__override __checkReturn HRESULT
CEventTrigger::ResolveTrigger(
    __in CUIElement* pObject,
    __in IStyleCallback* pCallback,
    __deref_out CResolvedTrigger** ppResolvedTrigger
    )
{
    HRESULT hr = S_OK;
    CResolvedEventTrigger* pResolvedEventTrigger = NULL;
    CTriggerActionCollection* pActions = NULL;
    CRoutedEvent* pRoutedEvent = NULL;

    IFCPTR(pObject);
    IFCPTR(ppResolvedTrigger);

    IFC(GetTypedEffectiveValue(&RoutedEventProperty, &pRoutedEvent));

    IFC(CResolvedEventTrigger::Create(pObject, pRoutedEvent, m_Providers, pCallback, &pResolvedEventTrigger));

    IFC(GetTypedEffectiveValue(&ActionsProperty, &pActions));

    for(UINT32 i = 0; i < pActions->GetCount(); i++)
    {
        CTriggerAction* pAction = pActions->GetAtIndex(i);

        IFC(pResolvedEventTrigger->AddAction(pAction));
    }

    *ppResolvedTrigger = pResolvedEventTrigger;
    pResolvedEventTrigger = NULL;

Cleanup:
    ReleaseObject(pResolvedEventTrigger);
    ReleaseObject(pActions);
    ReleaseObject(pRoutedEvent);

    return hr;
}

__checkReturn HRESULT 
CEventTrigger::CreatePropertyInformation(
    __deref_out CPropertyInformation **ppInformation
    )
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;

    CStaticProperty* Properties[] =
    {
        &RoutedEventProperty,
        &ActionsProperty
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
CEventTrigger::GetLayeredValue(
    __in CProperty* pProperty,
    __deref_out CLayeredValue** ppLayeredValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    if (pProperty->GetOwningType() == TypeIndex::EventTrigger)
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        switch(pStaticProperty->GetLocalIndex())
        {
            case EventTriggerProperties::RoutedEvent:
                {
                    *ppLayeredValue = &m_RoutedEvent;
                    break;
                }

            case EventTriggerProperties::Actions:
                {
                    *ppLayeredValue = &m_Actions;
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
        IFC_NOTRACE(CTrigger::GetLayeredValue(pProperty, ppLayeredValue));
    }

Cleanup:
    return hr;
}

CResolvedEventTrigger::CResolvedEventTrigger(
    ) 
    : m_pActions(NULL)
{
}

CResolvedEventTrigger::~CResolvedEventTrigger(
    )
{
    m_Connection.disconnect();
    ReleaseObject(m_pActions);
}

__checkReturn HRESULT
CResolvedEventTrigger::Initialize(
    __in CUIElement* pObject,
    __in CRoutedEvent* pRoutedEvent,
    __in CProviders* pProviders,
    __in IStyleCallback* pCallback
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pObject);
    IFCPTR(pProviders);
    IFCPTR(pRoutedEvent);

    IFC(CResolvedTriggerActions::Create(pObject, pProviders, pCallback, &m_pActions));

    IFC(pObject->AddHandler(pRoutedEvent, bind(&CResolvedEventTrigger::OnEvent, this, _1, _2), &m_Connection));

Cleanup:
    return hr;
}

__checkReturn HRESULT
CResolvedEventTrigger::AddAction(
    __in CTriggerAction* pAction
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pAction);

    IFC(m_pActions->AddAction(pAction));

Cleanup:
    return hr;
}

void
CResolvedEventTrigger::OnEvent(
    __in_opt CObjectWithType* pSender, 
    __in_opt CRoutedEventArgs* pRoutedEventArgs
    )
{
    HRESULT hr = S_OK;

    IFC(m_pActions->Apply());

Cleanup:
    ;
}