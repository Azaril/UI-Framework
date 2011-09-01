#include "DynamicResolvedClass.h"

CDynamicResolvedClass::CDynamicResolvedClass(
	) 
	: m_CachedProperties(NULL)
	, m_CachedEvents(NULL)
	, m_CachedCommands(NULL)
{
}

CDynamicResolvedClass::~CDynamicResolvedClass(
	)
{
    ReleaseObject(m_CachedProperties);
    ReleaseObject(m_CachedEvents);
    ReleaseObject(m_CachedCommands);
}

__checkReturn HRESULT 
CDynamicResolvedClass::Initialize(
	const ClassInformation& Info
	)
{
    HRESULT hr = S_OK;

    IFCPTR(Info.Name);

    m_Information = Info;

Cleanup:
    return hr;
}

__out const WCHAR* 
CDynamicResolvedClass::GetName(
	)
{
    return m_Information.Name;
}

TypeIndex::Value 
CDynamicResolvedClass::GetType(
	)
{
    return m_Information.ClassType;
}

__checkReturn HRESULT 
CDynamicResolvedClass::CreateInstance(
	__in CProviders* pProviders, 
	__deref_out CPropertyObject** ppObject
	)
{
    HRESULT hr = S_OK;

    IFCPTR(m_Information.CreateFunc);

    IFC(m_Information.CreateFunc(pProviders, ppObject));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CDynamicResolvedClass::GetPropertyInformation(
	__deref_out CPropertyInformation** ppInformation
	)
{
    HRESULT hr = S_OK;

    if(m_CachedProperties == NULL)
    {
        IFCPTR(m_Information.GetPropertiesFunc);

        IFC(m_Information.GetPropertiesFunc(&m_CachedProperties));
    }

    *ppInformation = m_CachedProperties;
    AddRefObject(m_CachedProperties);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CDynamicResolvedClass::GetEventInformation(
	__deref_out CEventInformation** ppInformation
	)
{
    HRESULT hr = S_OK;

    if(m_CachedEvents == NULL)
    {
        IFCPTR_NOTRACE(m_Information.GetEventsFunc);

        IFC(m_Information.GetEventsFunc(&m_CachedEvents));
    }

    *ppInformation = m_CachedEvents;
    AddRefObject(m_CachedEvents);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CDynamicResolvedClass::GetCommandInformation(
	__deref_out CCommandInformation** ppInformation
	)
{
    HRESULT hr = S_OK;

    if(m_CachedCommands == NULL)
    {
        IFCPTR(m_Information.GetCommandsFunc);

        IFC(m_Information.GetCommandsFunc(&m_CachedCommands));
    }

    *ppInformation = m_CachedCommands;
    AddRefObject(m_CachedCommands);

Cleanup:
    return hr;
}