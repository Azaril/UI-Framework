#pragma once

#include "RefCounted.h"
#include "Factory.h"
#include "Collections.h"
#include "PropertyObject.h"

class CNamedObjectHolder : public CRefCountedObject
{
    public:
        DECLARE_FACTORY2( CNamedObjectHolder, const WCHAR*, CObjectWithType* );

        __out const WCHAR* GetName(
            );

        __out CObjectWithType* GetObject(
            );

    protected:
        CNamedObjectHolder(
            );

        virtual ~CNamedObjectHolder(
            );

        __checkReturn HRESULT Initialize(
            __in_z const WCHAR* pName, 
            __in CObjectWithType* pObject 
            );

        std::wstring m_Name;
        CObjectWithType* m_Object;
};

class CNamescope : public CRefCountedObjectBase< CObjectWithType >
{
    public:
        DECLARE_FACTORY( CNamescope );

        DECLARE_TYPE_WITH_BASE( TypeIndex::Namescope, CObjectWithType );

        virtual __checkReturn HRESULT RegisterName(
            __in_z const WCHAR* pName, 
            __in CObjectWithType* pObject 
            );

        virtual __checkReturn HRESULT UnregisterName(
            __in_z const WCHAR* pName, 
            __in CObjectWithType* pObject 
            );

        virtual __checkReturn HRESULT FindName(
            __in_z const WCHAR* pName, 
            __deref_out_opt CObjectWithType** ppObject 
            );

    protected:
        CNamescope(
            );

        virtual ~CNamescope(
            );

        __checkReturn HRESULT InternalFindObject(
            __in_z const WCHAR* pName, 
            __deref_out_opt CNamedObjectHolder** ppHolder
            );

        __checkReturn HRESULT Initialize(
            );

        std::vector< CNamedObjectHolder* > m_NamedObjects;
};

template< >
struct ObjectTypeTraits< CNamescope >
{
    static const TypeIndex::Value Type = TypeIndex::Namescope;
};