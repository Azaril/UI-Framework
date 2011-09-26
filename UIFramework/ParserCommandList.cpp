#include "ParserCommandList.h"

CParserCommandList::CParserCommandList(
	) 
	: m_Providers(NULL)
{
}

CParserCommandList::~CParserCommandList(
	)
{
    for(std::vector< CParserCommand* >::iterator It = m_Commands.begin(); It != m_Commands.end(); ++It)
    {
        (*It)->Release();
    }

    m_Commands.clear();

    ReleaseObject(m_Providers);
}

__checkReturn HRESULT 
CParserCommandList::Initialize(
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
CParserCommandList::Execute(
	__in_opt IParserCallback* pCallback, 
	__deref_out CObjectWithType** ppObject
	)
{
    HRESULT hr = S_OK;
    CObjectWithType* pObject = NULL;

    CParserCommandContext Context(m_Providers, pCallback);

    for(std::vector< CParserCommand* >::iterator It = m_Commands.begin(); It != m_Commands.end(); ++It)
    {
        IFC((*It)->Execute(Context));
    }

    IFC(Context.GetObject(&pObject));

    //
    // Implicit pop of final object on stack.
    //
    if(pCallback)
    {
        IFC(pCallback->OnPopObject(pObject));
    }

    *ppObject = pObject;
    pObject = NULL;

Cleanup:
    ReleaseObject(pObject);

    return hr;
}

__checkReturn HRESULT 
CParserCommandList::AddCommand(
	__in CParserCommand* pCommand
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pCommand);

    m_Commands.push_back(pCommand);
    AddRefObject(pCommand);

Cleanup:
    return hr;
}