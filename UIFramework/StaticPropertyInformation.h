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
            const TypeIndex::Value OwningType,
            UINT32 LocalIndex,
            __in_z const WCHAR* Name,
            const TypeIndex::Value Type, 
            UINT32 Flags, 
            __in_opt GetDefaultPropertyValueFunc DefaultValueFunc = NULL, 
            __in_opt OnValueChangeFunc ValueChangeFunc = NULL
            );

        STATIC_REFCOUNTING();

        __override virtual TypeIndex::Value GetOwningType(
            );

        __override virtual TypeIndex::Value GetType(
            );

        __override virtual __out const WCHAR* GetName(
            );

        __override virtual bool IsCollection(
            );

        __override virtual bool IsDictionary(
            );

        __override virtual bool IsAttached(
            );

        __override virtual bool IsContent(
            );

        __override virtual bool IsReadOnly(
            );

        __override virtual HRESULT GetDefaultValue( 
            __deref_out_opt CObjectWithType** ppObject 
            );

        __override virtual __out_opt OnValueChangeFunc GetOnValueChangedCallback(
            );

        UINT32 GetLocalIndex(
            );

    protected:
        TypeIndex::Value m_OwningType;
        UINT32 m_LocalIndex;
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

        vector< CStaticProperty* > m_Properties;
};

#define DEFINE_GET_DEFAULT( name, type, value ) \
namespace   \
{   \
    CStaticBasicValue< type > g_defaultValue_##name(value); \
    \
    __checkReturn HRESULT GetDefault##name( \
        __deref_out_opt CObjectWithType** ppObject  \
        )   \
    {   \
        HRESULT hr = S_OK;  \
        \
        *ppObject = &g_defaultValue_##name; \
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
        *ppObject = NULL; \
        \
        return hr;  \
    }\
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
