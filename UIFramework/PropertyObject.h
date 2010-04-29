#pragma once

#include "Types.h"
#include "RefCounted.h"

namespace TypeIndex
{
    enum Value;
}

class CProperty : public CRefCountedObject
{
    public:
        virtual const WCHAR* GetName() = 0;
        virtual TypeIndex::Value GetType() = 0;
};

class CPropertyInformation : public CRefCountedObject
{
    public:
        virtual HRESULT GetProperty( const WCHAR* pPropertyName, CProperty** ppProperty ) = 0;
        virtual HRESULT GetContentProperty( CProperty** ppProperty ) = 0;
};

class CObjectWithType
{
    public:
        virtual INT32 AddRef() = 0;
        virtual INT32 Release() = 0;
        virtual TypeIndex::Value GetType() = 0;
};

class CPropertyObject
{
    public:
        virtual HRESULT GetPropertyInformation( CPropertyInformation** ppInformation ) = 0;

        virtual HRESULT SetValue( CProperty* pProperty, CObjectWithType* pValue ) = 0;

    protected:
        CPropertyObject();
        virtual ~CPropertyObject();
};
