#pragma once

#include "RefCounted.h"
#include "PropertyObject.h"

class CResolvedClass
{
    public:
        virtual INT32 AddRef() = 0;
        virtual INT32 Release() = 0;

        virtual HRESULT CreateInstance( CPropertyObject** ppObject ) = 0;
        virtual HRESULT GetPropertyInformation( CPropertyInformation** ppInformation ) = 0;
};

class CClassResolver : public CRefCountedObject
{
    public:
        virtual HRESULT ResolveType( const WCHAR* pTypeName, CResolvedClass** ppResolvedClass ) = 0;
};