#pragma once

#include "ClassResolver.h"
#include "Factory.h"

typedef HRESULT (*CreateTypeFunc)( CPropertyObject** ppObject );
typedef HRESULT (*GetPropertyInformationFunc)( CPropertyInformation** ppPropertyInformation );

class CStaticResolvedClass : public CResolvedClass
{
    public:
        CStaticResolvedClass( const WCHAR* pTypeName, CreateTypeFunc CreateFunc, GetPropertyInformationFunc GetPropertiesFunc );
        ~CStaticResolvedClass();

        virtual INT32 AddRef();
        virtual INT32 Release();

        const WCHAR* GetName();
        virtual HRESULT CreateInstance( CPropertyObject** ppObject );
        virtual HRESULT GetPropertyInformation( CPropertyInformation** ppInformation );

    protected:
        const WCHAR* m_Name;
        CreateTypeFunc m_CreateFunc;
        GetPropertyInformationFunc m_GetPropertiesFunc;
        CPropertyInformation* m_CachedProperties;
};

class CStaticClassResolver : public CClassResolver
{
    public:
        DECLARE_FACTORY( CStaticClassResolver );

        virtual HRESULT ResolveType( const WCHAR* pTypeName, CResolvedClass** ppResolvedClass );

    protected:
        CStaticClassResolver();
        virtual ~CStaticClassResolver();

        HRESULT Initialize();
};
