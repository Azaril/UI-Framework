#pragma once

#include "Types.h"
#include "RefCounted.h"
#include "TypeIndex.h"
#include "ErrorChecking.h"
#include "Signals.h"

template< typename T >
struct ObjectTypeTraits;

class CObjectWithType;
class CPropertyObject;
class CLayeredValue;

class CBindingContext;
class CBindingBase;

class CProviders;

namespace EffectiveValue
{
    enum Value
    {
        None        = 0x00,
        Default     = 0x01,
        Style       = 0x02,
        Local       = 0x04,
        Animation   = 0x08,

        Last        = Animation
    };
}

typedef __checkReturn HRESULT (*OnValueChangeFunc)( 
    __in CPropertyObject* pObjectInstance, 
    __in_opt CObjectWithType* pOldValue, 
    __in_opt CObjectWithType* pNewValue 
    );

class UIFRAMEWORK_API CProperty
{
    public:
        virtual INT32 AddRef(
            ) = 0;

        virtual INT32 Release(
            ) = 0;

        __out virtual const WCHAR* GetName(
            ) = 0;

        virtual TypeIndex::Value GetOwningType(
            ) = 0;

        virtual TypeIndex::Value GetType(
            ) = 0;

        virtual bool IsCollection(
            ) = 0;

        virtual bool IsDictionary(
            ) = 0;

        virtual bool IsAttached(
            ) = 0;

        virtual bool IsReadOnly(
            ) = 0;

        virtual __checkReturn HRESULT GetDefaultValue( 
            __deref_out_opt CObjectWithType** ppObject 
            ) = 0;

        virtual __out_opt OnValueChangeFunc GetOnValueChangedCallback(
            ) = 0;

        __checkReturn HRESULT OnValueChanged( 
            __in CPropertyObject* pObjectInstance, 
            __in_opt CObjectWithType* pOldValue, 
            __in_opt CObjectWithType* pNewValue 
            );
};

class UIFRAMEWORK_API CPropertyInformation : public CRefCountedObject
{
    public:
        virtual __checkReturn HRESULT GetProperty(
            __in_z const WCHAR* pPropertyName,
            __deref_out_opt CProperty** ppProperty
            ) = 0;

        virtual __checkReturn HRESULT GetContentProperty( 
            __deref_out_opt CProperty** ppProperty
            ) = 0;
};

//TODO: Add a base class to abstract routed events as they only make sense in the UI layer.
class CRoutedEvent;

class UIFRAMEWORK_API CEventInformation : public CRefCountedObject
{
    public:
        virtual __checkReturn HRESULT GetEvent( 
            __in_z const WCHAR* pEventName, 
            __deref_out_opt CRoutedEvent** ppRoutedEvent 
            ) = 0;
};

class CCommand;

class UIFRAMEWORK_API CCommandInformation : public CRefCountedObject
{
    public:
        virtual __checkReturn HRESULT GetCommand( 
            __in_z const WCHAR* pCommandName,
            __deref_out_opt CCommand** ppCommand 
            ) = 0;
};

#define DECLARE_TYPE( type ) \
__override virtual TypeIndex::Value GetType( \
    ) const \
{ \
    return type; \
} \
__override virtual bool IsTypeOf( \
    TypeIndex::Value Type \
    ) const \
{ \
    return Type == type; \
}

#define DECLARE_TYPE_WITH_BASE( type, base ) \
__override virtual TypeIndex::Value GetType( \
    ) const \
{ \
    return type; \
} \
__override virtual bool IsTypeOf( \
    TypeIndex::Value Type \
    ) const \
{ \
    return Type == type || base::IsTypeOf(Type); \
}

class UIFRAMEWORK_API CObjectWithType
{
    public:
        DECLARE_TYPE( TypeIndex::Object );

        virtual INT32 AddRef(
            ) = 0;

        virtual INT32 Release(
            ) = 0;       

        virtual bool Equals( 
            __in_opt CObjectWithType* pOther 
            )
        { 
            return this == pOther; 
        }

        virtual bool IsShareable(
            ) 
        { 
            return FALSE;
        }
};

template< >
struct ObjectTypeTraits< CObjectWithType >
{
    static const TypeIndex::Value Type = TypeIndex::Object;
};

class CAttachedPropertyHolder;

typedef events::signal< void ( CPropertyObject*, CProperty* ) > PropertyChangedSignal;
typedef PropertyChangedSignal::slot_type PropertyChangedHandler;

class UIFRAMEWORK_API CPropertyObject : public CObjectWithType
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::PropertyObject, CObjectWithType );

        virtual __checkReturn HRESULT SetValue(
            __in CProperty* pProperty, 
            __in CObjectWithType* pValue 
            );

        __checkReturn HRESULT SetValue(
            __in CProperty* pProperty,
            const FLOAT& value
            );

        __checkReturn HRESULT SetValue(
            __in CProperty* pProperty,
            const RectF& value
            );

        virtual __checkReturn HRESULT GetValue( 
            __in CProperty* pProperty, 
            __deref_out_opt CObjectWithType** ppValue 
            );

        virtual __checkReturn HRESULT GetEffectiveValue( 
            __in CProperty* pProperty, 
            __deref_out_opt CObjectWithType** ppValue 
            );

        __checkReturn HRESULT AddPropertyChangeListener(
            const PropertyChangedHandler& Handler, 
            __out events::signals::connection* pConnection 
            );

        __checkReturn HRESULT RaisePropertyChanged( 
            __in CProperty* pProperty 
            );

        virtual __checkReturn HRESULT SetBinding( 
            __in CProperty* pProperty, 
            __in CBindingBase* pBinding 
            );

        __checkReturn HRESULT SetBindingContext( 
            __in_opt CBindingContext* pContext 
            );

        __checkReturn HRESULT GetBindingContext( 
            __deref_out_opt CBindingContext** ppContext 
            );

        template< typename T >
        __checkReturn HRESULT GetTypedValue( 
            __in CProperty* pProperty,
            __deref_out_opt T** ppValue 
            )
        {
            HRESULT hr = S_OK;
            CObjectWithType* pVal = NULL;

            IFC(GetValue(pProperty, &pVal));

            if(pVal != NULL)
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

        template< typename T >
        __checkReturn HRESULT GetTypedEffectiveValue( 
            __in CProperty* pProperty,
            __deref_out_opt T** ppValue 
            )
        {
            HRESULT hr = S_OK;
            CObjectWithType* pVal = NULL;

            IFC(GetEffectiveValue(pProperty, &pVal));

            if(pVal != NULL)
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

        template< typename T >
        __checkReturn HRESULT GetBasicTypeEffectiveValue(
            __in CProperty* pProperty,
            __deref_out_opt T* pValue
            )
        {
            HRESULT hr = S_OK;
            CBasicValue< T >* pTypedValue = NULL;

            IFC(GetTypedEffectiveValue(pProperty, &pTypedValue));

            IFCPTR(pTypedValue);

            *pValue = pTypedValue->GetValue();

        Cleanup:
            ReleaseObject(pTypedValue);

            return hr;
        }

    protected:
        CPropertyObject(
            );

        virtual ~CPropertyObject(
            );

        __checkReturn HRESULT Initialize(
            __in CProviders* pProviders
            );

        virtual __checkReturn HRESULT SetValueReadOnly(
            __in CProperty* pProperty, 
            __in CObjectWithType* pValue 
            );

        __checkReturn HRESULT SetValueToLayer(
            EffectiveValue::Value layer,
            __in CProperty* pProperty,
            __in CObjectWithType* pValue
            );

        __checkReturn HRESULT ClearValueFromLayer(
            EffectiveValue::Value layer,
            __in CProperty* pProperty
            );

        __checkReturn HRESULT GetValueFromLayer(
            EffectiveValue::Value layer,
            __in CProperty* pProperty,
            __deref_out_opt CObjectWithType** ppValue
            );

        virtual __checkReturn HRESULT GetLayeredValue(
            __in CProperty* pProperty, 
            __deref_out_opt CLayeredValue** ppLayeredValue
            );

        __out CProviders* GetProviders(
            );

    private:
        __checkReturn HRESULT GetOrCreateAttachedLayeredValue(
            __in CProperty* pProperty,
            __out CLayeredValue** ppLayeredValue
            );

        CBindingContext* m_BindingContext;
        vector< CAttachedPropertyHolder* > m_AttachedProperties;
        PropertyChangedSignal m_PropertyChangedSignal;
        CProviders* m_pProviders;

#ifdef FRAMEWORK_DEBUG
        bool m_InitializeCalled;
#endif
};

template< >
struct ObjectTypeTraits< CPropertyObject >
{
    static const TypeIndex::Value Type = TypeIndex::PropertyObject;
};

class UIFRAMEWORK_API CObjectCollection : public CPropertyObject
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::Collection, CPropertyObject );

        virtual __checkReturn HRESULT AddObject( 
            __in CObjectWithType* pObject 
            ) = 0;

        virtual __checkReturn HRESULT RemoveObject(
            __in CObjectWithType* pObject 
            ) = 0;
};

template< >
struct ObjectTypeTraits< CObjectCollection >
{
    static const TypeIndex::Value Type = TypeIndex::Collection;
};

class UIFRAMEWORK_API CObjectDictionary : public CPropertyObject
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::Dictionary, CPropertyObject );

        virtual __checkReturn HRESULT AddObject( 
            __in CObjectWithType* pKey, 
            __in CObjectWithType* pObject 
            ) = 0;

        virtual __checkReturn HRESULT RemoveObject( 
            __in CObjectWithType* pKey, 
            __in CObjectWithType* pObject 
            ) = 0;
};

template< >
struct ObjectTypeTraits< CObjectDictionary >
{
    static const TypeIndex::Value Type = TypeIndex::Dictionary;
};

template< typename T >
inline 
__checkReturn HRESULT CastType(
    __in CObjectWithType* pBaseObject,
    __deref_out T** ppObject 
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pBaseObject);
    IFCPTR(ppObject);

    IFCEXPECT_NOTRACE(pBaseObject->IsTypeOf(ObjectTypeTraits< T >::Type));

    *ppObject = (T*)pBaseObject;

Cleanup:
    return hr;
}