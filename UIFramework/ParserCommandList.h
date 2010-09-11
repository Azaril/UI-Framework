#pragma once

#include "ParserCommand.h"
#include "Providers.h"

class CParserCommandList : public CRefCountedObjectBase< CObjectWithType >
{
    public:
        DECLARE_FACTORY1( CParserCommandList, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::ParserCommandList, CObjectWithType );

        virtual HRESULT Execute( IParserCallback* pCallback, CObjectWithType** ppObject );
        virtual HRESULT AddCommand( CParserCommand* pCommand );

    protected:
        CParserCommandList();
        virtual ~CParserCommandList();

        HRESULT Initialize( CProviders* pProviders );

        CProviders* m_Providers;
        std::vector< CParserCommand* > m_Commands;
};