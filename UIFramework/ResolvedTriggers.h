#pragma once

#include "RefCounted.h"
#include "Trigger.h"

class CResolvedTriggers : public CRefCountedObject
{
    public:
        DECLARE_FACTORY3( CResolvedTriggers, CUIElement*, CProviders*, IStyleCallback* );

        __checkReturn HRESULT AddTrigger(
            __in CTrigger* pTrigger 
            );

    protected:
        CResolvedTriggers(
            );

        virtual ~CResolvedTriggers(
            );

        __checkReturn HRESULT Initialize(
            __in CUIElement* pObject, 
            __in CProviders* pProviders, 
            __in IStyleCallback* pCallback 
            );

        CUIElement* m_Owner;
        CProviders* m_Providers;
        IStyleCallback* m_Callback;
        std::vector< CResolvedTrigger* > m_Triggers;
};