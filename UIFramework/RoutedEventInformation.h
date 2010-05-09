#pragma once

#include "PropertyObject.h"
#include "Factory.h"

class CRoutedEventInformation : public CEventInformation
{
    public:
        DECLARE_FACTORY2( CRoutedEventInformation, CRoutedEvent**, UINT32 );

        virtual HRESULT GetEvent( const WCHAR* pEventName, CRoutedEvent** ppEvent );

    public:
        CRoutedEventInformation();
        virtual ~CRoutedEventInformation();

        HRESULT Initialize( CRoutedEvent** ppEvents, UINT32 EventCount );

        std::vector< CRoutedEvent* > m_Events;
};