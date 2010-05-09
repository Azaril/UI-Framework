#pragma once

#include "RefCounted.h"
#include "Setter.h"

class CResolvedSetters : public CRefCountedObject
{
    public:
        DECLARE_FACTORY3( CResolvedSetters, CUIElement*, CProviders*, IStyleCallback* );

        HRESULT AddSetter( CSetter* pSetter );

        HRESULT Apply();

    protected:
        CResolvedSetters();
        virtual ~CResolvedSetters();

        HRESULT Initialize( CUIElement* pObject, CProviders* pProviders, IStyleCallback* pCallback );

        CUIElement* m_Owner;
        CProviders* m_Providers;
        IStyleCallback* m_Callback;
        std::vector< CResolvedSetter* > m_Setters;
};