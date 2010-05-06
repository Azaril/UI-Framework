#pragma once

#include "ClassResolver.h"
#include "Factory.h"

typedef HRESULT (*CreateTypeFunc)( CPropertyObject** ppObject );
typedef HRESULT (*GetPropertyInformationFunc)( CPropertyInformation** ppPropertyInformation );

class CStaticResolvedClass : public CResolvedClass
{
    public:
        CStaticResolvedClass( const WCHAR* pTypeName, TypeIndex::Value ClassType, CreateTypeFunc CreateFunc, GetPropertyInformationFunc GetPropertiesFunc );
        ~CStaticResolvedClass();

        virtual INT32 AddRef();
        virtual INT32 Release();

        const WCHAR* GetName();
        TypeIndex::Value GetType();
        virtual HRESULT CreateInstance( CPropertyObject** ppObject );
        virtual HRESULT GetPropertyInformation( CPropertyInformation** ppInformation );

    protected:
        const WCHAR* m_Name;
        TypeIndex::Value m_ClassType;
        CreateTypeFunc m_CreateFunc;
        GetPropertyInformationFunc m_GetPropertiesFunc;
        CPropertyInformation* m_CachedProperties;
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

    protected:
        CStaticClassResolver();
        virtual ~CStaticClassResolver();

        HRESULT Initialize();
};
