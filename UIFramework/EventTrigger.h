#pragma once

#include "Trigger.h"
#include "StaticPropertyInformation.h"
#include "RoutedEvent.h"
#include "RoutedEventArgs.h"
#include "StyleCallback.h"
#include "Setter.h"
#include "ResolvedSetters.h"

class CEventTrigger : public CTrigger
{
    public:
        DECLARE_FACTORY( CEventTrigger );

        DECLARE_TYPE_WITH_BASE( TypeIndex::EventTrigger, CTrigger );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT GetValue( CProperty* pProperty, CObjectWithType** ppValue );

        virtual HRESULT ResolveTrigger( CUIElement* pObject, CProviders* pProviders, IStyleCallback* pCallback, CResolvedTrigger** pResolvedTrigger );

        //
        // Properties
        //
        static CStaticProperty RoutedEventProperty;
        static CStaticProperty SettersProperty;

    protected:
        CEventTrigger();
        virtual ~CEventTrigger();

        HRESULT Initialize();

        virtual HRESULT SetValueInternal( CProperty* pProperty, CObjectWithType* pValue );

        CRoutedEvent* m_RoutedEvent;
        CSetterCollection* m_Setters;
};

template< >
struct ObjectTypeTraits< CEventTrigger >
{
    static const TypeIndex::Value Type = TypeIndex::EventTrigger;
};

class CResolvedEventTrigger : public CResolvedTrigger
{
    public:
        DECLARE_FACTORY4( CResolvedEventTrigger, CUIElement*, CRoutedEvent*, CProviders*, IStyleCallback* );

        HRESULT AddSetter( CSetter* pSetter );

    protected:
        CResolvedEventTrigger();
        virtual ~CResolvedEventTrigger();

        HRESULT Initialize( CUIElement* pObject, CRoutedEvent* pRoutedEvent, CProviders* pProviders, IStyleCallback* pCallback );

        void OnEvent( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );

        connection m_Connection;
        CResolvedSetters* m_Setters;
};