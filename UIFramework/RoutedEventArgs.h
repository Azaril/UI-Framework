#pragma once

#include "RefCounted.h"
#include "Factory.h"
#include "RoutedEvent.h"

class CRoutedEventArgs : public CRefCountedObject
{
    public:
        DECLARE_FACTORY1( CRoutedEventArgs, CRoutedEvent* );

        virtual CRoutedEvent* GetRoutedEvent();
        virtual BOOL IsHandled();
        virtual void SetHandled( BOOL Handled = TRUE );

    protected:
        CRoutedEventArgs();
        virtual ~CRoutedEventArgs();

        HRESULT Initialize( CRoutedEvent* pRoutedEvent );

        CRoutedEvent* m_RoutedEvent;
        BOOL m_Handled;
};
