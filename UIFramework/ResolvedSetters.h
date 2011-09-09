#pragma once

#include "RefCounted.h"
#include "Setter.h"

class CResolvedSetters : public CRefCountedObject
{
    public:
        DECLARE_FACTORY3( CResolvedSetters, CUIElement*, CProviders*, IStyleCallback* );

        __checkReturn HRESULT AddSetter( 
            __in CSetter* pSetter 
            );

        __checkReturn HRESULT Apply(
            );

    protected:
        CResolvedSetters(
            );

        virtual ~CResolvedSetters(
            );

        __checkReturn HRESULT Initialize(
            __in CUIElement* pObject, 
            __in CProviders* pProviders, 
            __in IStyleCallback* pCallback 
            );

        CUIElement* m_Owner;
        CProviders* m_Providers;
        IStyleCallback* m_Callback;
        std::vector< CResolvedSetter* > m_Setters;
};