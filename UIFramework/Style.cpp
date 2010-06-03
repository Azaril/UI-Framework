#include "Style.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"
#include "UIElement.h"

//
// Properties
//
CStaticProperty CStyle::SettersProperty( L"Setters", TypeIndex::Setter, StaticPropertyFlags::Collection | StaticPropertyFlags::Content | StaticPropertyFlags::ReadOnly );
CStaticProperty CStyle::TriggersProperty( L"Triggers", TypeIndex::Trigger, StaticPropertyFlags::Collection | StaticPropertyFlags::ReadOnly );

CStyle::CStyle() : m_Setters(NULL),
                   m_Triggers(NULL),
                   m_Providers(NULL)
{
}

CStyle::~CStyle()
{
    ReleaseObject(m_Setters);
    ReleaseObject(m_Triggers);
    ReleaseObject(m_Providers);
}

HRESULT CStyle::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFCPTR(pProviders);

    m_Providers = pProviders;
    AddRefObject(pProviders);

    IFC(CSetterCollection::Create(&m_Setters));
    IFC(CTriggerCollection::Create(&m_Triggers));

Cleanup:
    return hr;
}

HRESULT CStyle::ResolveStyle(CUIElement* pObject, IStyleCallback* pCallback, CResolvedStyle** ppResolvedStyle)
{
    HRESULT hr = S_OK;
    CResolvedSetters* pResolvedSetters = NULL;
    CResolvedTriggers* pResolvedTriggers = NULL;

    IFCPTR(pObject);
    IFCPTR(pCallback);

    IFC(ResolveSetters(pObject, pCallback, &pResolvedSetters));

    IFC(ResolveTriggers(pObject, pCallback, &pResolvedTriggers));

    IFC(CResolvedStyle::Create(pResolvedSetters, pResolvedTriggers, ppResolvedStyle));

    IFC(pResolvedSetters->Apply());

Cleanup:
    ReleaseObject(pResolvedSetters);
    ReleaseObject(pResolvedTriggers);

    return hr;
}

HRESULT CStyle::ResolveSetters(CUIElement* pObject, IStyleCallback* pCallback, CResolvedSetters** ppResolvedSetters)
{
    HRESULT hr = S_OK;
    CResolvedSetters* pResolvedSetters = NULL;

    IFCPTR(pObject);
    IFCPTR(ppResolvedSetters);

    IFC(CResolvedSetters::Create(pObject, m_Providers, pCallback, &pResolvedSetters));

    for(UINT32 i = 0; i < m_Setters->GetCount(); i++)
    {
        CSetter* pSetter = m_Setters->GetAtIndex(i);

        IFC(pResolvedSetters->AddSetter(pSetter));
    }

    *ppResolvedSetters = pResolvedSetters;
    pResolvedSetters = NULL;

Cleanup:
    ReleaseObject(pResolvedSetters);

    return hr;
}

HRESULT CStyle::ResolveTriggers(CUIElement* pObject, IStyleCallback* pCallback, CResolvedTriggers** ppResolvedTriggers)
{
    HRESULT hr = S_OK;
    CResolvedTriggers* pResolvedTriggers = NULL;

    IFCPTR(pObject);
    IFCPTR(ppResolvedTriggers);

    IFC(CResolvedTriggers::Create(pObject, m_Providers, pCallback, &pResolvedTriggers));

    for(UINT32 i = 0; i < m_Triggers->GetCount(); i++)
    {
        CTrigger* pTrigger = m_Triggers->GetAtIndex(i);

        IFC(pResolvedTriggers->AddTrigger(pTrigger));
    }

    *ppResolvedTriggers = pResolvedTriggers;
    pResolvedTriggers = NULL;

Cleanup:
    ReleaseObject(pResolvedTriggers);

    return hr;
}

HRESULT CStyle::CreatePropertyInformation(CPropertyInformation **ppInformation)
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

HRESULT CStyle::SetValueInternal(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    //if(pProperty == &CStyle::SettersProperty)
    //{
    //    IFC(E_NOTIMPL);
    //}
    //else
    //{
        IFC(CPropertyObject::SetValueInternal(pProperty, pValue));
    //}

Cleanup:
    return hr;
}

HRESULT CStyle::GetValue(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(pProperty == &CStyle::SettersProperty)
    {
        *ppValue = m_Setters;
        AddRefObject(m_Setters);
    }
    else if(pProperty == &CStyle::TriggersProperty)
    {
        *ppValue = m_Triggers;
        AddRefObject(m_Triggers);
    }
    else
    {
        IFC(CPropertyObject::GetValue(pProperty, ppValue));
    }

Cleanup:
    return hr;
}





CResolvedStyle::CResolvedStyle() : m_Setters(NULL),
                                   m_Triggers(NULL)
{
}

CResolvedStyle::~CResolvedStyle()
{
    ReleaseObject(m_Setters);
    ReleaseObject(m_Triggers);
}

HRESULT CResolvedStyle::Initialize(CResolvedSetters* pSetters, CResolvedTriggers* pTriggers)
{
    HRESULT hr = S_OK;

    IFCPTR(pSetters);
    IFCPTR(pTriggers);

    m_Setters = pSetters;
    AddRefObject(m_Setters);

    m_Triggers = pTriggers;
    AddRefObject(m_Triggers);

Cleanup:
    return hr;
}