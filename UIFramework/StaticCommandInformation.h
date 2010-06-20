#pragma once

#include "PropertyObject.h"
#include "Factory.h"
#include "Command.h"

class CStaticCommandInformation : public CCommandInformation
{
    public:
        DECLARE_FACTORY2( CStaticCommandInformation, CCommand**, UINT32 );

        virtual HRESULT GetCommand( const WCHAR* pCommandName, CCommand** ppCommand );

    protected:
        CStaticCommandInformation();
        virtual ~CStaticCommandInformation();

        HRESULT Initialize( CCommand** ppCommands, UINT32 CommandCount );

        std::vector< CCommand* > m_Commands;
};
