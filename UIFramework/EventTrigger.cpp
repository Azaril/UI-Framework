#include "EventTrigger.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"
#include "DelegatingPropertyInformation.h"
#include "UIElement.h"

//
// Properties
//
CStaticProperty CEventTrigger::RoutedEventProperty( L"RoutedEvent", TypeIndex::RoutedEvent, StaticPropertyFlags::None );
CStaticProperty CEventTrigger::SettersProperty( L"Setters", TypeIndex::Setter, StaticPropertyFlags::Collection | StaticPropertyFlags::Content | StaticPropertyFlags::ReadOnly );

CEventTrigger::CEventTrigger() : m_RoutedEvent(NULL),
                                 m_Setters(NULL)
{
}

CEventTrigger::~CEventTrigger()
{
    ReleaseObject(m_RoutedEvent);
    ReleaseObject(m_Setters);
}

HRESULT CEventTrigger::Initialize()
{
    HRESULT hr = S_OK;

    IFC(CSetterCollection::Create(&m_Setters));

Cleanup:
    return hr;
}

HRESULT CEventTrigger::ResolveTrigger(CUIElement* pObject, CProviders* pProviders, IStyleCallback* pCallback, CResolvedTrigger** ppResolvedTrigger)
{
    HRESULT hr = S_OK;
    CResolvedEventTrigger* pResolvedEventTrigger = NULL;

    IFCPTR(pObject);
    IFCPTR(pProviders);
    IFCPTR(ppResolvedTrigger);

    IFC(CResolvedEventTrigger::Create(pObject, m_RoutedEvent, pProviders, pCallback, &pResolvedEventTrigger));

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

HRESULT CEventTrigger::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;

    IFCPTR(ppInformation);

    CStaticProperty* Properties[] =
    {
        &RoutedEventProperty,
        &SettersProperty
    };

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));

    *ppInformation = pStaticInformation;
    pStaticInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);

    return hr;
}

HRESULT CEventTrigger::SetValueInternal(CProperty* pProperty, CObjectWithType* pValue)
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

HRESULT CEventTrigger::GetValue(CProperty* pProperty, CObjectWithType** ppValue)
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




CResolvedEventTrigger::CResolvedEventTrigger() : m_Setters(NULL)
{
}

CResolvedEventTrigger::~CResolvedEventTrigger()
{
    m_Connection.disconnect();
    ReleaseObject(m_Setters);
}

HRESULT CResolvedEventTrigger::Initialize(CUIElement* pObject, CRoutedEvent* pRoutedEvent, CProviders* pProviders, IStyleCallback* pCallback)
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

HRESULT CResolvedEventTrigger::AddSetter(CSetter* pSetter)
{
    HRESULT hr = S_OK;

    IFCPTR(pSetter);

    IFC(m_Setters->AddSetter(pSetter));

Cleanup:
    return hr;
}

void CResolvedEventTrigger::OnEvent(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;

    IFC(m_Setters->Apply());

Cleanup:
    ;
}