#pragma once

#include "RefCounted.h"
#include "Factory.h"
#include "PropertyObject.h"

namespace RoutingStrategy
{
    enum Value
    {
        Direct,
        Bubbling,
        Tunneled
    };
}

class CRoutedEvent : public CObjectWithType
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::RoutedEvent, CObjectWithType );

        virtual __out const WCHAR* GetName(
            ) = 0;

        virtual RoutingStrategy::Value GetRoutingStrategy(
            ) = 0;
};

template< >
struct ObjectTypeTraits< CRoutedEvent >
{
    static const TypeIndex::Value Type = TypeIndex::RoutedEvent;
};