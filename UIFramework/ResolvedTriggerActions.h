#pragma once

#include "RefCounted.h"
#include "TriggerAction.h"
#include "Providers.h"

class CUIElement;
class CProviders;
struct IStyleCallback;

class CResolvedTriggerActions : public CRefCountedObject
{
    public:
        DECLARE_FACTORY3( CResolvedTriggerActions, CUIElement*, CProviders*, IStyleCallback* );

        __checkReturn HRESULT AddAction( 
            __in CTriggerAction* pAction
            );

        __checkReturn HRESULT Apply(
            );

    protected:
        CResolvedTriggerActions(
            );

        virtual ~CResolvedTriggerActions(
            );

        __checkReturn HRESULT Initialize(
            __in CUIElement* pObject, 
            __in CProviders* pProviders, 
            __in IStyleCallback* pCallback 
            );

        CUIElement* m_Owner;
        CProviders* m_Providers;
        IStyleCallback* m_Callback;
        vector< CResolvedTriggerAction* > m_Actions;
};