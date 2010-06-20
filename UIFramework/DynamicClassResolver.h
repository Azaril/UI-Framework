#pragma once

#include "ClassResolver.h"
#include "Factory.h"
#include "Providers.h"
#include "DynamicResolvedClass.h"

class CDynamicClassResolver : public CClassResolver
{
    typedef std::vector< CDynamicResolvedClass* > ClassContainer;

    public:
        DECLARE_FACTORY( CDynamicClassResolver );

        HRESULT RegisterClass( const ClassInformation& Info );

        virtual HRESULT ResolveType( TypeIndex::Value ClassType, CResolvedClass** ppResolvedClass );
        virtual HRESULT ResolveType( const WCHAR* pTypeName, CResolvedClass** ppResolvedClass );

        virtual HRESULT ResolveProperties( TypeIndex::Value ClassType, CPropertyInformation** ppProperties );
        virtual HRESULT ResolveProperties( const WCHAR* pTypeName, CPropertyInformation** ppProperties );

        virtual HRESULT ResolveProperty( const WCHAR* pPropertyName, TypeIndex::Value ImplicitClass, CProperty** ppProperty );
        virtual HRESULT ResolveProperty( const WCHAR* pPropertyName, CPropertyInformation* pImplicitClassProperties, CProperty** ppProperty );

        virtual HRESULT ResolveEvents( TypeIndex::Value ClassType, CEventInformation** ppEvents );
        virtual HRESULT ResolveEvents( const WCHAR* pTypeName, CEventInformation** ppEvents );

        virtual HRESULT ResolveEvent( const WCHAR* pEventName, TypeIndex::Value ImplicitClass, CRoutedEvent** ppRoutedEvent );
        virtual HRESULT ResolveEvent( const WCHAR* pEventName, CEventInformation* pImplicitClassEvents, CRoutedEvent** ppRoutedEvent );

        virtual HRESULT ResolveCommands( TypeIndex::Value ClassType, CCommandInformation** ppCommands );
        virtual HRESULT ResolveCommands( const WCHAR* pTypeName, CCommandInformation** ppCommands );

        virtual HRESULT ResolveCommand( const WCHAR* pCommandName, TypeIndex::Value ImplicitClass, CCommand** ppCommand );
        virtual HRESULT ResolveCommand( const WCHAR* pCommandName, CCommandInformation* pImplicitClassCommands, CCommand** ppCommand );

    protected:
        CDynamicClassResolver();
        virtual ~CDynamicClassResolver();

        HRESULT Initialize();

        std::vector< CDynamicResolvedClass* > m_Classes;
};

HRESULT LoadDefaultTypes( CDynamicClassResolver* pClassResolver );