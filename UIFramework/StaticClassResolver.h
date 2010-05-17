#pragma once

#include "ClassResolver.h"
#include "Factory.h"
#include "Providers.h"

typedef HRESULT (*CreateTypeFunc)( CProviders* pProviders, CPropertyObject** ppObject );
typedef HRESULT (*GetPropertyInformationFunc)( CPropertyInformation** ppPropertyInformation );
typedef HRESULT (*GetEventInformationFunc)( CEventInformation** ppEventInformation );

class CStaticResolvedClass : public CResolvedClass
{
    public:
        CStaticResolvedClass( const WCHAR* pTypeName, TypeIndex::Value ClassType, CreateTypeFunc CreateFunc, GetPropertyInformationFunc GetPropertiesFunc, GetEventInformationFunc GetEventsFunc = NULL );
        ~CStaticResolvedClass();

        virtual INT32 AddRef();
        virtual INT32 Release();

        const WCHAR* GetName();
        virtual TypeIndex::Value GetType();

        virtual HRESULT CreateInstance( CProviders* pProviders, CPropertyObject** ppObject );

        virtual HRESULT GetPropertyInformation( CPropertyInformation** ppInformation );
        virtual HRESULT GetEventInformation( CEventInformation** ppEventInformation );

    protected:
        const WCHAR* m_Name;
        TypeIndex::Value m_ClassType;
        CreateTypeFunc m_CreateFunc;
        GetPropertyInformationFunc m_GetPropertiesFunc;
        GetEventInformationFunc m_GetEventsFunc;
        CPropertyInformation* m_CachedProperties;
        CEventInformation* m_CachedEvents;
};

class CStaticClassResolver : public CClassResolver
{
    public:
        DECLARE_FACTORY( CStaticClassResolver );

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

    protected:
        CStaticClassResolver();
        virtual ~CStaticClassResolver();

        HRESULT Initialize();
};
