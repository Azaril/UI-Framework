#pragma once

#include "Trigger.h"
#include "StaticPropertyInformation.h"
#include "RoutedEvent.h"
#include "RoutedEventArgs.h"
#include "StyleCallback.h"
#include "Setter.h"
#include "ResolvedTriggerActions.h"
#include "LayeredValue.h"

class CEventTrigger : public CTrigger
{
    public:
        DECLARE_FACTORY1( CEventTrigger, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::EventTrigger, CTrigger );

        static __checkReturn HRESULT CreatePropertyInformation( 
            __deref_out CPropertyInformation** ppInformation 
            );

        __override virtual __checkReturn HRESULT ResolveTrigger( 
            __in CUIElement* pObject, 
            __in IStyleCallback* pCallback, 
            __deref_out CResolvedTrigger** pResolvedTrigger 
            );

        //
        // Properties
        //
        static CStaticProperty RoutedEventProperty;
        static CStaticProperty ActionsProperty;

    protected:
        CEventTrigger(
            );

        virtual ~CEventTrigger(
            );

        __checkReturn HRESULT Initialize(
            __in CProviders* pProviders 
            );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        CLayeredValue m_RoutedEvent;
        CLayeredValue m_Actions;
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

        __checkReturn HRESULT AddAction(
            __in CTriggerAction* pAction
            );

    protected:
        CResolvedEventTrigger(
            );

        virtual ~CResolvedEventTrigger(
            );

        __checkReturn HRESULT Initialize( 
            __in CUIElement* pObject,
            __in CRoutedEvent* pRoutedEvent,
            __in CProviders* pProviders,
            __in IStyleCallback* pCallback 
            );

        void OnEvent( 
            __in_opt CObjectWithType* pSender, 
            __in_opt CRoutedEventArgs* pRoutedEventArgs 
            );

        events::signals::connection m_Connection;
        CResolvedTriggerActions* m_pActions;
};