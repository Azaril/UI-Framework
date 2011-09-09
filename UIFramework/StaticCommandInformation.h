#pragma once

#include "PropertyObject.h"
#include "Factory.h"
#include "Command.h"

class CStaticCommandInformation : public CCommandInformation
{
    public:
        DECLARE_FACTORY2( CStaticCommandInformation, CCommand**, UINT32 );

        __override virtual __checkReturn HRESULT GetCommand( 
            __in_z const WCHAR* pCommandName,
            __deref_out_opt CCommand** ppCommand 
            );

    protected:
        CStaticCommandInformation(
            );

        virtual ~CStaticCommandInformation(
            );

        __checkReturn HRESULT Initialize( 
            __in_ecount(CommandCount) CCommand** ppCommands, 
            UINT32 CommandCount 
            );

        std::vector< CCommand* > m_Commands;
};
