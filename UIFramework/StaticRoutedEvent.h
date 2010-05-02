#pragma once

#include "RoutedEvent.h"

template< RoutingStrategy::Value Strategy >
class CStaticRoutedEvent : public CRoutedEvent
{
    public:
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
};
