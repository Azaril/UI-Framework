#pragma once

#include "Types.h"
#include "RefCounted.h"
#include "TypeIndex.h"

namespace TypeIndex
{
    enum Value;
}

template< typename T >
struct ObjectTypeTraits;

class CProperty
{
    public:
        virtual INT32 AddRef() = 0;
        virtual INT32 Release() = 0;

        virtual const WCHAR* GetName() = 0;
        virtual TypeIndex::Value GetType() = 0;
        virtual BOOL IsCollection() = 0;
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

        virtual TypeIndex::Value GetType() { return TypeIndex::Object; }
        virtual BOOL IsTypeOf( TypeIndex::Value Type ) { return Type == TypeIndex::Object; }
};

template< >
struct ObjectTypeTraits< CObjectWithType >
{
    static const TypeIndex::Value Type = TypeIndex::Object;
};

class CPropertyObject : public CObjectWithType
{
    public:
        virtual HRESULT GetPropertyInformation( CPropertyInformation** ppInformation ) = 0;

        virtual HRESULT SetValue( CProperty* pProperty, CObjectWithType* pValue ) = 0;
        virtual HRESULT GetValue( CProperty* pProperty, CObjectWithType** ppValue ) = 0;

    protected:
        CPropertyObject();
        virtual ~CPropertyObject();
};

class CObjectCollection : public CObjectWithType
{
    public:
        virtual TypeIndex::Value GetType() { return TypeIndex::Collection; }
        virtual BOOL IsTypeOf( TypeIndex::Value Type ) { return Type == TypeIndex::Collection || CObjectWithType::IsTypeOf(Type); }

        virtual HRESULT AddObject( CObjectWithType* pObject ) = 0;
        virtual HRESULT RemoveObject( CObjectWithType* pObject ) = 0;
};

template< >
struct ObjectTypeTraits< CObjectCollection >
{
    static const TypeIndex::Value Type = TypeIndex::Collection;
};