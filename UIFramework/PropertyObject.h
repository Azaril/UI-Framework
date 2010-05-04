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
        virtual BOOL IsAttached() = 0;
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

class CAttachedPropertyHolder
{
    public:
        CAttachedPropertyHolder( const CAttachedPropertyHolder& Other );
        CAttachedPropertyHolder( CProperty* pProperty, CObjectWithType* pObject );
        ~CAttachedPropertyHolder();

        HRESULT SetValue( CObjectWithType* pObject );
        HRESULT GetValue( CObjectWithType** ppObject );

        CProperty* GetProperty();

    protected:
        CProperty* m_Property;
        CObjectWithType* m_Value;
};

class CPropertyObject : public CObjectWithType
{
    public:
        virtual HRESULT SetValue( CProperty* pProperty, CObjectWithType* pValue );
        virtual HRESULT GetValue( CProperty* pProperty, CObjectWithType** ppValue );

        template< typename T >
        HRESULT GetTypedValue( CProperty* pProperty, T** ppValue )
        {
            HRESULT hr = S_OK;
            CObjectWithType* pVal = NULL;

            IFC(GetValue(pProperty, &pVal));

            if(pVal)
            {
                IFCEXPECT(pVal->IsTypeOf(ObjectTypeTraits< T >::Type));

                *ppValue = (T*)pVal;
                pVal = NULL;
            }
            else
            {
                *ppValue = NULL;
            }

        Cleanup:
            ReleaseObject(pVal);

            return hr;
        }

    protected:
        CPropertyObject();
        virtual ~CPropertyObject();

        std::vector< CAttachedPropertyHolder > m_AttachedProperties;
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

#define DECLARE_TYPE( type ) \
virtual TypeIndex::Value GetType() { return type; } \
virtual BOOL IsTypeOf( TypeIndex::Value Type ) { return Type == type; }

#define DECLARE_TYPE_WITH_BASE( type, base ) \
virtual TypeIndex::Value GetType() { return type; } \
virtual BOOL IsTypeOf( TypeIndex::Value Type ) { return Type == type || base::IsTypeOf(Type); }