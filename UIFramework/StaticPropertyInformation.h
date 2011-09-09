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

typedef __checkReturn HRESULT (*GetDefaultPropertyValueFunc)( 
    __deref_out_opt CObjectWithType** ppObject 
    );

class UIFRAMEWORK_API CStaticProperty : public CProperty
{
    public:
        CStaticProperty( 
            __in_z const WCHAR* Name,
            const TypeIndex::Value Type, 
            UINT32 Flags, 
            __in_opt GetDefaultPropertyValueFunc DefaultValueFunc = NULL, 
            __in_opt OnValueChangeFunc ValueChangeFunc = NULL
            );

        __override virtual INT32 AddRef(
            );

        __override virtual INT32 Release(
            );

        __override virtual TypeIndex::Value GetType(
            );

        __override virtual __out const WCHAR* GetName(
            );

        __override virtual BOOL IsCollection(
            );

        __override virtual BOOL IsDictionary(
            );

        __override virtual BOOL IsAttached(
            );

        __override virtual BOOL IsContent(
            );

        __override virtual BOOL IsReadOnly(
            );

        __override virtual HRESULT GetDefaultValue( 
            __deref_out_opt CObjectWithType** ppObject 
            );

        __override virtual __out_opt OnValueChangeFunc GetOnValueChangedCallback(
            );

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

        __override virtual __checkReturn HRESULT GetProperty(
            __in const WCHAR* pPropertyName,
            __deref_out_opt CProperty** ppProperty 
            );

        __override virtual __checkReturn HRESULT GetContentProperty(
            __deref_out_opt CProperty** ppProperty 
            );

    public:
        CStaticPropertyInformation(
            );

        virtual ~CStaticPropertyInformation(
            );

        __checkReturn HRESULT Initialize(
            __in_ecount(PropertyCount) CStaticProperty** ppProperties,
            UINT32 PropertyCount 
            );

        std::vector< CStaticProperty* > m_Properties;
};

// HACK: Remove this as it triggers an allocation for every default value query.
#define DEFINE_GET_DEFAULT( name, type, value ) \
namespace   \
{   \
    __checkReturn HRESULT GetDefault##name( \
        __deref_out_opt CObjectWithType** ppObject  \
            )   \
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
    __checkReturn HRESULT GetDefault##name( \
        __deref_out_opt CObjectWithType** ppObject  \
            )    \
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
__checkReturn HRESULT   \
type::Static##name( \
    __in CPropertyObject* pObjectInstance,  \
    __in_opt CObjectWithType* pOldValue,    \
    __in_opt CObjectWithType* pNewValue \
    )    \
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
static __checkReturn HRESULT Static##name(  \
    __in CPropertyObject* pObjectInstance,  \
    __in_opt CObjectWithType* pOldValue,    \
    __in_opt CObjectWithType* pNewValue \
    );

#define INSTANCE_CHANGE_CALLBACK( type, name ) type::Static##name
