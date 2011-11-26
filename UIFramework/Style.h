#pragma once

#include "PropertyObject.h"
#include "RefCounted.h"
#include "Factory.h"
#include "Collection.h"
#include "Setter.h"
#include "Providers.h"
#include "StaticPropertyInformation.h"
#include "Trigger.h"
#include "ResolvedTriggers.h"
#include "ResolvedTriggerActions.h"

class CResolvedStyle;

class CStyle : public CRefCountedObjectBase< CPropertyObject >
{
    public:
        DECLARE_FACTORY1( CStyle, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::Style, CPropertyObject );

        static __checkReturn HRESULT CreatePropertyInformation( 
            __deref_out CPropertyInformation** ppInformation 
            );

        __override virtual bool IsShareable(
            ) 
        { 
            return TRUE; 
        }

        __checkReturn HRESULT ResolveStyle( 
            __in CUIElement* pObject,
            __in IStyleCallback* pCallback, 
            __deref_out CResolvedStyle** ppResolvedStyle 
            );

        //
        // Properties
        //
        static CStaticProperty SettersProperty;
        static CStaticProperty TriggersProperty;

    protected:
        CStyle(
            );

        virtual ~CStyle(
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

        __checkReturn HRESULT ResolveSetters( 
            __in CUIElement* pObject, 
            __in IStyleCallback* pCallback,
            __deref_out CResolvedTriggerActions** ppResolvedActions
            );

        __checkReturn HRESULT ResolveTriggers(
            __in CUIElement* pObject, 
            __in IStyleCallback* pCallback, 
            __deref_out CResolvedTriggers** ppResolvedTriggers 
            );

        CProviders* m_Providers;
        CSetterCollection* m_Setters;
        CTriggerCollection* m_Triggers;
};

template< >
struct ObjectTypeTraits< CStyle >
{
    static const TypeIndex::Value Type = TypeIndex::Style;
};

class CResolvedStyle : public CRefCountedObject
{
    public:
        DECLARE_FACTORY2( CResolvedStyle, CResolvedTriggerActions*, CResolvedTriggers* );
        
    protected:
        CResolvedStyle(
            );

        virtual ~CResolvedStyle(
            );

        __checkReturn HRESULT Initialize(
            __in CResolvedTriggerActions* pActions, 
            __in CResolvedTriggers* pTriggers 
            );

        CResolvedTriggerActions* m_Actions;
        CResolvedTriggers* m_Triggers;
};