#pragma once

#include "ParserCommand.h"
#include "Providers.h"

class CParserCommandList : public CRefCountedObject
{
    public:
        DECLARE_FACTORY1( CParserCommandList, CProviders* );

        virtual HRESULT Execute( CObjectWithType** ppObject );
        virtual HRESULT AddCommand( CParserCommand* pCommand );

    protected:
        CParserCommandList();
        virtual ~CParserCommandList();

        HRESULT Initialize( CProviders* pProviders );

        CProviders* m_Providers;
        std::vector< CParserCommand* > m_Commands;
};