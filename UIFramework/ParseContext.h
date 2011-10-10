#pragma once

#include "ClassResolver.h"
#include "TypeConverter.h"
#include "Factory.h"
#include "ParserCommand.h"
#include "ParserCommandList.h"

class CParseContext : public CRefCountedObject
{
    public:
        DECLARE_FACTORY1( CParseContext, CProviders* );

        __out CProviders* GetProviders(
			);

        __out CClassResolver* GetClassResolver(
			);

        __out CTypeConverter* GetTypeConverter(
			);
        
        __checkReturn HRESULT GetCommandList( 
			__deref_out CParserCommandList** ppCommandList 
			);

        __checkReturn HRESULT PushCommandList( 
			__in CParserCommandList* pCommandList 
			);

        __checkReturn HRESULT PopCommandList(
			);

        __checkReturn HRESULT AddCommand( 
			__in CParserCommand* pCommand 
			);

    protected:
        CParseContext(
			);

        virtual ~CParseContext(
			);

        __checkReturn HRESULT Initialize( 
			__in CProviders* pProviders 
			);

        CProviders* m_Providers;
        CClassResolver* m_ClassResolver;
        CTypeConverter* m_TypeConverter;

        vector< CParserCommandList* > m_CommandLists;
};