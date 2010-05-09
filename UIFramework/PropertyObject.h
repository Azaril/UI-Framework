#pragma once

#include "Types.h"
#include "RefCounted.h"
#include "TypeIndex.h"
#include "ErrorChecking.h"

template< typename T >
struct ObjectTypeTraits;

class CObjectWithType;
class CPropertyObject;

class CProperty
{
    public:
        virtual INT32 AddRef() = 0;
        virtual INT32 Release() = 0;

        virtual const WCHAR* GetName() = 0;
        virtual TypeIndex::Value GetType() = 0;
        virtual BOOL IsCollection() = 0;
        virtual BOOL IsDictionary() = 0;
        virtual BOOL IsAttached() = 0;
        virtual BOOL IsReadOnly() = 0;

        virtual HRESULT GetDefaultValue( CObjectWithType** ppObject ) = 0;

        virtual HRESULT OnValueChanged( CPropertyObject* pObjectInstance, CObjectWithType* pOldValue, CObjectWithType* pNewValue ) = 0;
};

class CPropertyInformation : public CRefCountedObject
{
    public:
        virtual HRESULT GetProperty( const WCHAR* pPropertyName, CProperty** ppProperty ) = 0;
        virtual HRESULT GetContentProperty( CProperty** ppProperty ) = 0;
};

//TODO: Add a base class to abstract routed events as they only make sense in the UI layer.
class CRoutedEvent;

class CEventInformation : public CRefCountedObject
{
    public:
        virtual HRESULT GetEvent( const WCHAR* pEventName, CRoutedEvent** ppRoutedEvent ) = 0;
};

#define DECLARE_TYPE( type ) \
virtual TypeIndex::Value GetType() { return type; } \
virtual BOOL IsTypeOf( TypeIndex::Value Type ) { return Type == type; }

#define DECLARE_TYPE_WITH_BASE( type, base ) \
virtual TypeIndex::Value GetType() { return type; } \
virtual BOOL IsTypeOf( TypeIndex::Value Type ) { return Type == type || base::IsTypeOf(Type); }

class CObjectWithType
{
    public:
        virtual INT32 AddRef() = 0;
        virtual INT32 Release() = 0;

        DECLARE_TYPE( TypeIndex::Object );

        virtual BOOL Equals( CObjectWithType* pOther ) { return this == pOther; }
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
        DECLARE_TYPE_WITH_BASE( TypeIndex::PropertyObject, CObjectWithType );

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

        virtual HRESULT SetValueInternal( CProperty* pProperty, CObjectWithType* pValue );

        std::vector< CAttachedPropertyHolder > m_AttachedProperties;
};

class CObjectCollection : public CObjectWithType
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::Collection, CObjectWithType );

        virtual HRESULT AddObject( CObjectWithType* pObject ) = 0;
        virtual HRESULT RemoveObject( CObjectWithType* pObject ) = 0;
};

template< >
struct ObjectTypeTraits< CObjectCollection >
{
    static const TypeIndex::Value Type = TypeIndex::Collection;
};

class CObjectDictionary : public CObjectWithType
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::Dictionary, CObjectWithType );

        virtual HRESULT AddObject( CObjectWithType* pKey, CObjectWithType* pObject ) = 0;
        virtual HRESULT RemoveObject( CObjectWithType* pKey, CObjectWithType* pObject ) = 0;
};

template< >
struct ObjectTypeTraits< CObjectDictionary >
{
    static const TypeIndex::Value Type = TypeIndex::Dictionary;
};

template< typename T >
inline HRESULT CastType( CObjectWithType* pBaseObject, T** ppObject )
{
    HRESULT hr = S_OK;

    IFCPTR(ppObject);

    if(pBaseObject)
    {
        IFCEXPECT(pBaseObject->IsTypeOf(ObjectTypeTraits< T >::Type));

        *ppObject = (T*)pBaseObject;
    }
    else
    {
        *ppObject = NULL;
    }

Cleanup:
    return hr;
}