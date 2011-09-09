#include "EventTrigger.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"
#include "DelegatingPropertyInformation.h"
#include "UIElement.h"

//
// Properties
//
CStaticProperty CEventTrigger::RoutedEventProperty(L"RoutedEvent", TypeIndex::RoutedEvent, StaticPropertyFlags::None);
CStaticProperty CEventTrigger::SettersProperty(L"Setters", TypeIndex::Setter, StaticPropertyFlags::Collection | StaticPropertyFlags::Content | StaticPropertyFlags::ReadOnly);

CEventTrigger::CEventTrigger(
    ) 
    : m_RoutedEvent(NULL)
    , m_Setters(NULL)
    , m_Providers(NULL)
{
}

CEventTrigger::~CEventTrigger(
    )
{
    ReleaseObject(m_RoutedEvent);
    ReleaseObject(m_Setters);
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

    IFC(CSetterCollection::Create(&m_Setters));

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

    IFCPTR(pObject);
    IFCPTR(ppResolvedTrigger);

    IFC(CResolvedEventTrigger::Create(pObject, m_RoutedEvent, m_Providers, pCallback, &pResolvedEventTrigger));

    for(UINT32 i = 0; i < m_Setters->GetCount(); i++)
    {
        CSetter* pSetter = m_Setters->GetAtIndex(i);

        IFC(pResolvedEventTrigger->AddSetter(pSetter));
    }

    *ppResolvedTrigger = pResolvedEventTrigger;
    pResolvedEventTrigger = NULL;

Cleanup:
    ReleaseObject(pResolvedEventTrigger);

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
        &SettersProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));

    *ppInformation = pStaticInformation;
    pStaticInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);

    return hr;
}

__checkReturn HRESULT
CEventTrigger::SetValueInternal(
    __in CProperty* pProperty, 
    __in CObjectWithType* pValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    if(pProperty == &CEventTrigger::RoutedEventProperty)
    {
        CRoutedEvent* pRoutedEvent = NULL;

        IFC(CastType(pValue, &pRoutedEvent));

        ReleaseObject(m_RoutedEvent);

        m_RoutedEvent = pRoutedEvent;

        AddRefObject(m_RoutedEvent);
    }
    else
    {
        IFC(CPropertyObject::SetValueInternal(pProperty, pValue));
    }

Cleanup:
    return hr;
}

__override __checkReturn HRESULT
CEventTrigger::GetValue(
    __in CProperty* pProperty, 
    __deref_out_opt CObjectWithType** ppValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(pProperty == &CEventTrigger::RoutedEventProperty)
    {
        *ppValue = m_RoutedEvent;
        AddRefObject(m_RoutedEvent);
    }
    else if(pProperty == &CEventTrigger::SettersProperty)
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

CResolvedEventTrigger::CResolvedEventTrigger(
    ) 
    : m_Setters(NULL)
{
}

CResolvedEventTrigger::~CResolvedEventTrigger(
    )
{
    m_Connection.disconnect();
    ReleaseObject(m_Setters);
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

    IFC(CResolvedSetters::Create(pObject, pProviders, pCallback, &m_Setters));

    IFC(pObject->AddHandler(pRoutedEvent, bind(&CResolvedEventTrigger::OnEvent, this, _1, _2), &m_Connection));

Cleanup:
    return hr;
}

__checkReturn HRESULT
CResolvedEventTrigger::AddSetter(
    __in CSetter* pSetter
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pSetter);

    IFC(m_Setters->AddSetter(pSetter));

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

    IFC(m_Setters->Apply());

Cleanup:
    ;
}