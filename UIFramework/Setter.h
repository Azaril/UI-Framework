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

        __out_opt const WCHAR* GetPropertyName(
            );

        __out_opt CObjectWithType* GetPropertyValue(
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

        __override virtual __checkReturn HRESULT SetValueInternal(
            __in CProperty* pProperty,
            __in CObjectWithType* pValue 
            );

        __override virtual __checkReturn HRESULT GetValueInternal(
            __in CProperty* pProperty, 
            __deref_out_opt CObjectWithType** ppValue 
            );

        __checkReturn HRESULT SetPropertyInternal( 
            __in  CStringValue* pProperty 
            );

        __checkReturn HRESULT SetPropertyValueInternal( 
            __in CParserCommandList* pValue 
            );

        CProviders* m_Providers;
        CStringValue* m_Property;
        CParserCommandList* m_Value;
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
        DECLARE_FACTORY( CSetterCollection );

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