#include "StaticCommandInformation.h"

CStaticCommandInformation::CStaticCommandInformation()
{
}

CStaticCommandInformation::~CStaticCommandInformation()
{
    for(std::vector< CCommand* >::iterator It = m_Commands.begin(); It != m_Commands.end(); ++It)
    {
        (*It)->Release();
    }

    m_Commands.clear();
}

HRESULT CStaticCommandInformation::Initialize(CCommand** ppCommands, UINT32 CommandCount)
{
    HRESULT hr = S_OK;

    if(CommandCount > 0)
    {
        IFCPTR(ppCommands);
    }

    for(UINT32 i = 0; i < CommandCount; i++)
    {
        CCommand* pCommand = ppCommands[i];

        IFCPTR(pCommand);

        m_Commands.push_back(pCommand);

        AddRefObject(pCommand);
    }

Cleanup:
    return hr;
}

HRESULT CStaticCommandInformation::GetCommand(const WCHAR* pCommandName, CCommand** ppCommand)
{
    HRESULT hr = S_OK;

    IFCPTR(pCommandName);
    IFCPTR(ppCommand);

    for(std::vector< CCommand* >::iterator It = m_Commands.begin(); It != m_Commands.end(); ++It)
    {
        CCommand* pCommand = (*It);

        if(wcscmp(pCommandName, pCommand->GetName()) == 0)
        {
            *ppCommand = pCommand;
            AddRefObject(pCommand);

            goto Cleanup;
        }
    }

    hr = E_FAIL;

Cleanup:
    return hr;
}