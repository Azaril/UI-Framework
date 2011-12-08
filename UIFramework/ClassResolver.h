#pragma once

#include "RefCounted.h"
#include "PropertyObject.h"

class CProviders;

class UIFRAMEWORK_API CResolvedClass
{
    public:
        DECLARE_ADDREF_RELEASE_INTERFACE();

        virtual TypeIndex::Value GetType(
			) = 0;

        virtual __checkReturn HRESULT CreateInstance(
			__in CProviders* pProviders, 
			__deref_out CPropertyObject** ppObject 
			) = 0;

        virtual __checkReturn HRESULT GetPropertyInformation( 
			__deref_out CPropertyInformation** ppInformation 
			) = 0;

        virtual __checkReturn HRESULT GetEventInformation( 
			__deref_out CEventInformation** ppInformation
			) = 0;

        virtual __checkReturn HRESULT GetCommandInformation( 
			__deref_out CCommandInformation** ppInformation
			) = 0;
};

class UIFRAMEWORK_API CClassResolver : public CRefCountedObject
{
    public:
        virtual __checkReturn HRESULT ResolveType(
			TypeIndex::Value ClassType, 
			__deref_out CResolvedClass** ppResolvedClass 
			) = 0;

        virtual __checkReturn HRESULT ResolveType( 
			__in_z const WCHAR* pTypeName, 
			__deref_out CResolvedClass** ppResolvedClass
			) = 0;

        virtual __checkReturn HRESULT ResolveProperties( 
			TypeIndex::Value ClassType, 
			__deref_out CPropertyInformation** ppProperties
			) = 0;

        virtual __checkReturn HRESULT ResolveProperties( 
			__in_z const WCHAR* pTypeName, 
			__deref_out CPropertyInformation** ppProperties 
			) = 0;

        virtual __checkReturn HRESULT ResolveProperty( 
			__in_z const WCHAR* pPropertyName, 
			TypeIndex::Value ImplicitClass, 
			__deref_out CProperty** ppProperty 
			) = 0;

        virtual __checkReturn HRESULT ResolveProperty( 
			__in_z const WCHAR* pPropertyName, 
			__in_opt CPropertyInformation* pImplicitClassProperties, 
			__deref_out CProperty** ppProperty
			) = 0;

        virtual __checkReturn HRESULT ResolveEvents(
			TypeIndex::Value ClassType, 
			__deref_out CEventInformation** ppEvents 
			) = 0;

        virtual __checkReturn HRESULT ResolveEvents( 
			__in_z const WCHAR* pTypeName, 
			__deref_out CEventInformation** ppEvents
			) = 0;

        virtual __checkReturn HRESULT ResolveEvent( 
			__in_z const WCHAR* pEventName, 
			TypeIndex::Value ImplicitClass, 
			__deref_out CRoutedEvent** ppRoutedEvent 
			) = 0;

        virtual __checkReturn HRESULT ResolveEvent( 
			__in_z const WCHAR* pEventName,
			__in_opt CEventInformation* pImplicitClassEvents, 
			__deref_out CRoutedEvent** ppRoutedEvent 
			) = 0;

        virtual __checkReturn HRESULT ResolveCommands( 
			TypeIndex::Value ClassType,
			__deref_out CCommandInformation** ppCommands 
			) = 0;

        virtual __checkReturn HRESULT ResolveCommands(
			__in_z const WCHAR* pTypeName, 
			__deref_out CCommandInformation** ppCommands
			) = 0;

        virtual __checkReturn HRESULT ResolveCommand(
			__in_z const WCHAR* pCommandName, 
			TypeIndex::Value ImplicitClass, 
			__deref_out CCommand** ppCommand 
			) = 0;

        virtual __checkReturn HRESULT ResolveCommand(
			__in_z const WCHAR* pCommandName,
			__in_opt CCommandInformation* pImplicitClassCommands,
			__deref_out CCommand** ppCommand
			) = 0;
};