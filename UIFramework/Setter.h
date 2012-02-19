#pragma once

#include "PropertyObject.h"
#include "RefCounted.h"
#include "Factory.h"
#include "BasicTypes.h"
#include "StaticPropertyInformation.h"
#include "Collection.h"
#include "Providers.h"
#include "StyleCallback.h"
#include "ParserCommandList.h"
#include "TriggerAction.h"
#include "LayeredValue.h"

class CResolvedSetter;
class CUIElement;

class CSetter : public CRefCountedObjectBase< CTriggerAction >
{
    public:
        DECLARE_FACTORY1( CSetter, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::Setter, CTriggerAction );

        static __checkReturn HRESULT CreatePropertyInformation( 
            __deref_out CPropertyInformation** ppInformation
            );

        virtual __checkReturn HRESULT ResolveAction(
            __in CUIElement* pObject,
            __in IStyleCallback* pCallback,
            __deref_out CResolvedTriggerAction** ppResolvedAction 
            );

        //
        // Properties
        //
        static CStaticProperty PropertyProperty;
        static CStaticProperty ValueProperty;

    protected:
        CSetter(
            );

        virtual ~CSetter(
            );

        __checkReturn HRESULT Initialize(
            __in CProviders* pProviders 
            );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnValueChanged );

        __checkReturn HRESULT OnValueChanged(
            __in_opt CObjectWithType* pOldValue,
            __in_opt CObjectWithType* pNewValue 
            );

        CLayeredValue m_Property;
        CLayeredValue m_Value;
        CObjectWithType* m_CachedValue;
};

template< >
struct ObjectTypeTraits< CSetter >
{
    static const TypeIndex::Value Type = TypeIndex::Setter;
};

class CSetterCollection : public CCollection< CSetter >
{
    public:
        DECLARE_FACTORY1( CSetterCollection, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::SetterCollection, CCollection< CSetter > );
};

template< >
struct ObjectTypeTraits< CSetterCollection >
{
    static const TypeIndex::Value Type = TypeIndex::SetterCollection;
};

class CResolvedSetter : public CResolvedTriggerAction
{
    public:
        DECLARE_FACTORY3( CResolvedSetter, CProperty*, CObjectWithType*, IStyleCallback* );

        virtual __checkReturn HRESULT Apply(
            );

    protected:
        CResolvedSetter(
            );

        virtual ~CResolvedSetter(
            );

        __checkReturn HRESULT Initialize(
            __in CProperty* pProperty, 
            __in CObjectWithType* pValue, 
            __in IStyleCallback* pCallback 
            );

        CProperty* m_Property;
        CObjectWithType* m_Value;
        IStyleCallback* m_Callback;
};