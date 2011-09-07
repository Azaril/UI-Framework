#pragma once

#include "RefCounted.h"
#include "Factory.h"
#include "RoutedEvent.h"
#include "PropertyObject.h"

class CRoutedEventArgs : public CRefCountedObjectBase< CObjectWithType >
{
    public:
        DECLARE_FACTORY1( CRoutedEventArgs, CRoutedEvent* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::RoutedEventArgs, CObjectWithType );

        virtual CRoutedEvent* GetRoutedEvent(
            );

        virtual BOOL IsHandled(
            );

        virtual void SetHandled(
            BOOL Handled = TRUE 
            );

    protected:
        CRoutedEventArgs(
            );

        virtual ~CRoutedEventArgs(
            );

        __checkReturn HRESULT Initialize( 
            __in CRoutedEvent* pRoutedEvent 
            );

        CRoutedEvent* m_RoutedEvent;
        BOOL m_Handled;
};

template< >
struct ObjectTypeTraits< CRoutedEventArgs >
{
    static const TypeIndex::Value Type = TypeIndex::RoutedEventArgs;
};