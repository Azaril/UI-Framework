#pragma once

#include "ParserCommand.h"
#include "Providers.h"

class CParserCommandList : public CRefCountedObjectBase< CObjectWithType >
{
    public:
        DECLARE_FACTORY1( CParserCommandList, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::ParserCommandList, CObjectWithType );

        virtual HRESULT Execute(
			__in_opt IParserCallback* pCallback, 
			__deref_out CObjectWithType** ppObject 
			);

        virtual HRESULT AddCommand(
			__in CParserCommand* pCommand 
			);

    protected:
        CParserCommandList(
			);

        virtual ~CParserCommandList(
			);

        __checkReturn HRESULT Initialize(
			CProviders* pProviders 
			);

        CProviders* m_Providers;
        vector< CParserCommand* > m_Commands;
};

template< >
struct ObjectTypeTraits< CParserCommandList >
{
    static const TypeIndex::Value Type = TypeIndex::ParserCommandList;
};