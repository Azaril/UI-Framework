#pragma once

#include "RefCounted.h"
#include "Factory.h"

namespace RoutingStrategy
{
    enum Value
    {
        Direct,
        Bubbling
    };
}

class CRoutedEvent
{
    public:
        virtual INT32 AddRef() = 0;
        virtual INT32 Release() = 0;

        virtual RoutingStrategy::Value GetRoutingStrategy() = 0;
};
