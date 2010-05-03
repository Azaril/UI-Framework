#pragma once

#include "RoutedEventArgs.h"

class CInputEventArgs : public CRoutedEventArgs
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::InputEventArgs, CRoutedEventArgs );
};

template< >
struct ObjectTypeTraits< CInputEventArgs >
{
    static const TypeIndex::Value Type = TypeIndex::InputEventArgs;
};