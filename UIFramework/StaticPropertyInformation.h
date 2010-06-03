#pragma once

#include "PropertyObject.h"
#include "Factory.h"

namespace StaticPropertyFlags
{
    enum Value
    {
        None        = 0x00,
        Content     = 0x01,
        Collection  = 0x02,
        Dictionary  = 0x04,
        Attached    = 0x08,
        ReadOnly    = 0x10
    };
}

typedef HRESULT (*GetDefaultPropertyValueFunc)( CObjectWithType** ppObject );
typedef HRESULT (*OnValueChangeFunc)( CPropertyObject* pObjectInstance, CObjectWithType* pOldValue, CObjectWithType* pNewValue );

class CStaticProperty : public CProperty
{
    public:
        CStaticProperty( const WCHAR* Name, const TypeIndex::Value Type, UINT32 Flags, GetDefaultPropertyValueFunc DefaultValueFunc = NULL, OnValueChangeFunc ValueChangeFunc = NULL );

        virtual INT32 AddRef();
        virtual INT32 Release();

        virtual TypeIndex::Value GetType();
        virtual const WCHAR* GetName();
        virtual BOOL IsCollection();
        virtual BOOL IsDictionary();
        virtual BOOL IsAttached();
        virtual BOOL IsContent();
        virtual BOOL IsReadOnly();

        virtual HRESULT GetDefaultValue( CObjectWithType** ppObject );

        virtual HRESULT OnValueChanged( CPropertyObject* pObjectInstance, CObjectWithType* pOldValue, CObjectWithType* pNewValue );

    protected:
        const WCHAR* m_Name;
        TypeIndex::Value m_Type;
        UINT32 m_Flags;
        GetDefaultPropertyValueFunc m_DefaultValueFunc;
        OnValueChangeFunc m_ValueChangeFunc;
};

class CStaticPropertyInformation : public CPropertyInformation
{
    public:
        DECLARE_FACTORY2( CStaticPropertyInformation, CStaticProperty**, UINT32 );

        virtual HRESULT GetProperty( const WCHAR* pPropertyName, CProperty** ppProperty );
        virtual HRESULT GetContentProperty( CProperty** ppProperty );

    public:
        CStaticPropertyInformation();
        virtual ~CStaticPropertyInformation();

        HRESULT Initialize( CStaticProperty** ppProperties, UINT32 PropertyCount );

        std::vector< CStaticProperty* > m_Properties;
};

// HACK: Remove this as it triggers an allocation for every default value query.
#define DEFINE_GET_DEFAULT( name, type, value ) \
namespace   \
{   \
    HRESULT GetDefault##name(CObjectWithType** ppObject)    \
    {   \
        HRESULT hr = S_OK;  \
        type* pValue = NULL;    \
        \
        IFCPTR(ppObject);   \
        \
        IFC( type::Create(value, &pValue) );    \
        \
        *ppObject = pValue; \
        pValue = NULL;  \
        \
    Cleanup:    \
        ReleaseObject(pValue);  \
        \
        return hr;  \
    }   \
}

#define DEFINE_GET_DEFAULT_NULL( name ) \
namespace   \
{   \
    HRESULT GetDefault##name(CObjectWithType** ppObject)    \
    {   \
        HRESULT hr = S_OK;  \
        \
        IFCPTR(ppObject);   \
        \
        *ppObject = NULL; \
        \
    Cleanup:    \
        return hr;  \
    }   \
}

#define GET_DEFAULT( name ) GetDefault##name

#define DEFINE_INSTANCE_CHANGE_CALLBACK( type, name ) \
HRESULT type::Static##name(CPropertyObject* pObjectInstance, CObjectWithType* pOldValue, CObjectWithType* pNewValue)    \
{   \
    HRESULT hr = S_OK;  \
    type* pTypedInstance = NULL;    \
    \
    IFCPTR(pObjectInstance);   \
    \
    IFCEXPECT(pObjectInstance->IsTypeOf(ObjectTypeTraits< type >::Type));   \
    \
    pTypedInstance = (type*)pObjectInstance;  \
    \
    IFC(pTypedInstance->name(pOldValue, pNewValue));  \
    \
Cleanup:    \
    return hr;  \
}

#define DECLARE_INSTANCE_CHANGE_CALLBACK( name )    \
static HRESULT Static##name(CPropertyObject* pObjectInstance, CObjectWithType* pOldValue, CObjectWithType* pNewValue);

#define INSTANCE_CHANGE_CALLBACK( type, name ) type::Static##name
