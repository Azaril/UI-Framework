#pragma once

#include "RefCounted.h"
#include "Trigger.h"

class CResolvedTriggers : public CRefCountedObject
{
    public:
        DECLARE_FACTORY3( CResolvedTriggers, CUIElement*, CProviders*, IStyleCallback* );

        HRESULT AddTrigger( CTrigger* pTrigger );

    protected:
        CResolvedTriggers();
        virtual ~CResolvedTriggers();

        HRESULT Initialize( CUIElement* pObject, CProviders* pProviders, IStyleCallback* pCallback );

        CUIElement* m_Owner;
        CProviders* m_Providers;
        IStyleCallback* m_Callback;
        std::vector< CResolvedTrigger* > m_Triggers;
};