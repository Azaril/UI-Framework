#include "StaticCommandInformation.h"

CStaticCommandInformation::CStaticCommandInformation(
    )
{
}

CStaticCommandInformation::~CStaticCommandInformation(
    )
{
    for(vector< CCommand* >::iterator It = m_Commands.begin(); It != m_Commands.end(); ++It)
    {
        (*It)->Release();
    }

    m_Commands.clear();
}

__checkReturn HRESULT 
CStaticCommandInformation::Initialize(
    __in_ecount(CommandCount) CCommand** ppCommands, 
    UINT32 CommandCount
    )
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

__override __checkReturn HRESULT 
CStaticCommandInformation::GetCommand(
    __in_z const WCHAR* pCommandName,
    __deref_out_opt CCommand** ppCommand
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pCommandName);
    IFCPTR(ppCommand);

    for(vector< CCommand* >::iterator It = m_Commands.begin(); It != m_Commands.end(); ++It)
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