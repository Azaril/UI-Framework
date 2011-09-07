#pragma once

#include "RefCounted.h"
#include "Factory.h"
#include "RoutedEvent.h"
#include "PropertyObject.h"
#include "RoutedEventArgs.h"
#include "RoutedCommand.h"

class CRoutedCommandEventArgs : public CRoutedEventArgs
{
    public:
        DECLARE_FACTORY2( CRoutedCommandEventArgs, CRoutedEvent*, CRoutedCommand* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::RoutedCommandEventArgs, CRoutedEventArgs );

        __out CRoutedCommand* GetCommand(
            );

    protected:
        CRoutedCommandEventArgs(
            );

        virtual ~CRoutedCommandEventArgs(
            );

        __checkReturn HRESULT Initialize(
            __in CRoutedEvent* pRoutedEvent,
            __in CRoutedCommand* pCommand 
            );

        CRoutedCommand* m_Command;
};

template< >
struct ObjectTypeTraits< CRoutedCommandEventArgs >
{
    static const TypeIndex::Value Type = TypeIndex::RoutedCommandEventArgs;
};