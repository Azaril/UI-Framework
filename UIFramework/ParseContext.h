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

        CProviders* GetProviders();
        CClassResolver* GetClassResolver();
        CTypeConverter* GetTypeConverter();
        
        HRESULT GetCommandList( CParserCommandList** ppCommandList );
        HRESULT PushCommandList( CParserCommandList* pCommandList );
        HRESULT PopCommandList();

        HRESULT AddCommand( CParserCommand* pCommand );

    protected:
        CParseContext();
        virtual ~CParseContext();

        HRESULT Initialize( CProviders* pProviders );

        CProviders* m_Providers;
        CClassResolver* m_ClassResolver;
        CTypeConverter* m_TypeConverter;

        std::vector< CParserCommandList* > m_CommandLists;
};