#pragma once

#include "RoutedEvent.h"

template< RoutingStrategy::Value Strategy >
class CStaticRoutedEvent : public CRoutedEvent
{
    public:
        CStaticRoutedEvent( const WCHAR* pName ) : m_Name(pName)
        {
        }

        virtual INT32 AddRef()
        {
            return 1;
        }

        virtual INT32 Release()
        {
            return 1;
        }

        virtual RoutingStrategy::Value GetRoutingStrategy()
        {
            return Strategy;
        }

        virtual const WCHAR* GetName()
        {
            return m_Name;
        }

    protected:
        const WCHAR* m_Name;
};
