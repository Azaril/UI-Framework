#include "ParserCommandList.h"

CParserCommandList::CParserCommandList() : m_Providers(NULL)
{
}

CParserCommandList::~CParserCommandList()
{
    for(std::vector< CParserCommand* >::iterator It = m_Commands.begin(); It != m_Commands.end(); ++It)
    {
        (*It)->Release();
    }

    m_Commands.clear();

    ReleaseObject(m_Providers);
}

HRESULT CParserCommandList::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFCPTR(pProviders);

    m_Providers = pProviders;
    AddRefObject(m_Providers);

Cleanup:
    return hr;
}

HRESULT CParserCommandList::Execute(CObjectWithType** ppObject)
{
    HRESULT hr = S_OK;

    CParserCommandContext Context(m_Providers);

    for(std::vector< CParserCommand* >::iterator It = m_Commands.begin(); It != m_Commands.end(); ++It)
    {
        IFC((*It)->Execute(Context));
    }

    IFC(Context.GetObject(ppObject));

Cleanup:
    return hr;
}

HRESULT CParserCommandList::AddCommand(CParserCommand* pCommand)
{
    HRESULT hr = S_OK;

    IFCPTR(pCommand);

    m_Commands.push_back(pCommand);
    AddRefObject(pCommand);

Cleanup:
    return hr;
}