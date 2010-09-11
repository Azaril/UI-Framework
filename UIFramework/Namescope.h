#pragma once

#include "RefCounted.h"
#include "Factory.h"
#include "Collections.h"
#include "PropertyObject.h"

class CNamedObjectHolder : public CRefCountedObject
{
    public:
        DECLARE_FACTORY2( CNamedObjectHolder, const WCHAR*, CObjectWithType* );

        const WCHAR* GetName();
        CObjectWithType* GetObject();

    protected:
        CNamedObjectHolder();
        virtual ~CNamedObjectHolder();

        HRESULT Initialize( const WCHAR* pName, CObjectWithType* pObject );

        std::wstring m_Name;
        CObjectWithType* m_Object;
};

class CNamescope : public CRefCountedObjectBase< CObjectWithType >
{
    public:
        DECLARE_FACTORY( CNamescope );

        DECLARE_TYPE_WITH_BASE( TypeIndex::Namescope, CObjectWithType );

        virtual HRESULT RegisterName( const WCHAR* pName, CObjectWithType* pObject );
        virtual HRESULT UnregisterName( const WCHAR* pName, CObjectWithType* pObject );

        virtual HRESULT FindName( const WCHAR* pName, CObjectWithType** ppObject );

    protected:
        CNamescope();
        virtual ~CNamescope();

        HRESULT InternalFindObject(const WCHAR* pName, CNamedObjectHolder** ppHolder);

        HRESULT Initialize();

        std::vector< CNamedObjectHolder* > m_NamedObjects;
};

template< >
struct ObjectTypeTraits< CNamescope >
{
    static const TypeIndex::Value Type = TypeIndex::Namescope;
};