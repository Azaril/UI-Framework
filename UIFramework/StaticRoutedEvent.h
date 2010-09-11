#pragma once

#include "RoutedEvent.h"

class CStaticRoutedEvent : public CRoutedEvent
{
    public:
        CStaticRoutedEvent( const WCHAR* pName, RoutingStrategy::Value Strategy );

        virtual INT32 AddRef();
        virtual INT32 Release();

        virtual RoutingStrategy::Value GetRoutingStrategy();
        virtual const WCHAR* GetName();

    protected:
        const WCHAR* m_Name;
        RoutingStrategy::Value m_Strategy;
};
