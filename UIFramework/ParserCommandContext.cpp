#include "ParserCommandContext.h"

CParserCommandContext::CParserCommandContext(
	__in CProviders* pProviders, 
	__in IParserCallback* pCallback
	) 
	: m_Providers(pProviders)
	, m_Callback(pCallback)
{
    AddRefObject(m_Providers);
}

CParserCommandContext::~CParserCommandContext(
	)
{
    for(vector< CObjectWithType* >::iterator It = m_ObjectStack.begin(); It != m_ObjectStack.end(); ++It)
    {
        (*It)->Release();
    }

    ReleaseObject(m_Providers);
}

__out CProviders* 
CParserCommandContext::GetProviders(
	)
{
    return m_Providers;
}

__checkReturn HRESULT 
CParserCommandContext::GetObject(
	__deref_out CObjectWithType** ppObject
	)
{
    HRESULT hr = S_OK;

    IFCPTR(ppObject);

    IFCEXPECT(!m_ObjectStack.empty());

    SetObject(*ppObject, m_ObjectStack.back());

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CParserCommandContext::PushObject(
	__in CObjectWithType* pObject
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pObject);

    m_ObjectStack.push_back(pObject);
    AddRefObject(pObject);

    if(m_Callback)
    {
        IFC(m_Callback->OnPushObject(pObject));
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CParserCommandContext::PopObject(
	)
{
    HRESULT hr = S_OK;
    CObjectWithType* pObject = NULL;

    IFCEXPECT(!m_ObjectStack.empty());

    pObject = m_ObjectStack.back();

    if(m_Callback)
    {
        IFC(m_Callback->OnPopObject(pObject));
    }

    m_ObjectStack.pop_back();

    ReleaseObject(pObject);

Cleanup:
    return hr;
}