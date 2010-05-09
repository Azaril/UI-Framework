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
#include "ResolvedSetters.h"

class CResolvedStyle;

class CStyle : public CRefCountedObjectBase< CPropertyObject >
{
    public:
        DECLARE_FACTORY( CStyle );

        DECLARE_TYPE_WITH_BASE( TypeIndex::Style, CPropertyObject );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT GetValue( CProperty* pProperty, CObjectWithType** ppValue );

        HRESULT ResolveStyle( CUIElement* pObject, CProviders* pProviders, IStyleCallback* pCallback, CResolvedStyle** ppResolvedStyle );

        //
        // Properties
        //
        static CStaticProperty SettersProperty;
        static CStaticProperty TriggersProperty;

    protected:
        CStyle();
        virtual ~CStyle();

        HRESULT Initialize();

        virtual HRESULT SetValueInternal( CProperty* pProperty, CObjectWithType* pValue );

        HRESULT ResolveSetters( CUIElement* pObject, CProviders* pProviders, IStyleCallback* pCallback, CResolvedSetters** ppResolvedSetters );
        HRESULT ResolveTriggers( CUIElement* pObject, CProviders* pProviders, IStyleCallback* pCallback, CResolvedTriggers** ppResolvedTriggers );

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
        DECLARE_FACTORY2( CResolvedStyle, CResolvedSetters*, CResolvedTriggers* );
        
    protected:
        CResolvedStyle();
        virtual ~CResolvedStyle();

        HRESULT Initialize( CResolvedSetters* pSetters, CResolvedTriggers* pTriggers );

        CResolvedSetters* m_Setters;
        CResolvedTriggers* m_Triggers;
};