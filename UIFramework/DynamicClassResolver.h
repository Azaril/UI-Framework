#pragma once

#include "ClassResolver.h"
#include "Factory.h"
#include "Providers.h"
#include "DynamicResolvedClass.h"

class UIFRAMEWORK_API CDynamicClassResolver : public CClassResolver
{
    typedef vector< CDynamicResolvedClass* > ClassContainer;

    public:
        DECLARE_FACTORY( CDynamicClassResolver );

        __checkReturn HRESULT RegisterClass( 
			const ClassInformation& Info 
			);

        __override virtual __checkReturn HRESULT ResolveType(
			TypeIndex::Value ClassType, 
			__deref_out CResolvedClass** ppResolvedClass 
			);

        __override virtual __checkReturn HRESULT ResolveType( 
			__in_z const WCHAR* pTypeName, 
			__deref_out CResolvedClass** ppResolvedClass
			);

        __override virtual __checkReturn HRESULT ResolveProperties( 
			TypeIndex::Value ClassType, 
			__deref_out CPropertyInformation** ppProperties
			);

        __override virtual __checkReturn HRESULT ResolveProperties( 
			__in_z const WCHAR* pTypeName, 
			__deref_out CPropertyInformation** ppProperties 
			);

        __override virtual __checkReturn HRESULT ResolveProperty( 
			__in_z const WCHAR* pPropertyName, 
			TypeIndex::Value ImplicitClass, 
			__deref_out CProperty** ppProperty 
			);

        __override virtual __checkReturn HRESULT ResolveProperty( 
			__in_z const WCHAR* pPropertyName, 
			__in_opt CPropertyInformation* pImplicitClassProperties, 
			__deref_out CProperty** ppProperty
			);

        __override virtual __checkReturn HRESULT ResolveEvents(
			TypeIndex::Value ClassType, 
			__deref_out CEventInformation** ppEvents 
			);

        __override virtual __checkReturn HRESULT ResolveEvents( 
			__in_z const WCHAR* pTypeName, 
			__deref_out CEventInformation** ppEvents
			);

        __override virtual __checkReturn HRESULT ResolveEvent( 
			__in_z const WCHAR* pEventName, 
			TypeIndex::Value ImplicitClass, 
			__deref_out CRoutedEvent** ppRoutedEvent 
			);

        __override virtual __checkReturn HRESULT ResolveEvent( 
			__in_z const WCHAR* pEventName,
			__in_opt CEventInformation* pImplicitClassEvents, 
			__deref_out CRoutedEvent** ppRoutedEvent 
			);

        __override virtual __checkReturn HRESULT ResolveCommands( 
			TypeIndex::Value ClassType,
			__deref_out CCommandInformation** ppCommands 
			);

        __override virtual __checkReturn HRESULT ResolveCommands(
			__in_z const WCHAR* pTypeName, 
			__deref_out CCommandInformation** ppCommands
			);

        __override virtual __checkReturn HRESULT ResolveCommand(
			__in_z const WCHAR* pCommandName, 
			TypeIndex::Value ImplicitClass, 
			__deref_out CCommand** ppCommand 
			);

        __override virtual __checkReturn HRESULT ResolveCommand(
			__in_z const WCHAR* pCommandName,
			__in_opt CCommandInformation* pImplicitClassCommands,
			__deref_out CCommand** ppCommand
			);

    protected:
        CDynamicClassResolver(
			);

        virtual ~CDynamicClassResolver(
			);

        __checkReturn HRESULT Initialize(
			);

        vector< CDynamicResolvedClass* > m_Classes;
};

__checkReturn HRESULT LoadDefaultTypes( 
	__in CDynamicClassResolver* pClassResolver 
	);