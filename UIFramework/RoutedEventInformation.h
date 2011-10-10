#pragma once

#include "PropertyObject.h"
#include "Factory.h"

class CRoutedEventInformation : public CEventInformation
{
    public:
        DECLARE_FACTORY2( CRoutedEventInformation, CRoutedEvent**, UINT32 );

        __override virtual __checkReturn HRESULT GetEvent( 
            __in_z const WCHAR* pEventName,
            __deref_out_opt CRoutedEvent** ppEvent 
            );

    public:
        CRoutedEventInformation(
            );

        virtual ~CRoutedEventInformation(
            );

        __checkReturn HRESULT Initialize(
            __in_ecount(EventCount) CRoutedEvent** ppEvents,
            UINT32 EventCount 
            );

        vector< CRoutedEvent* > m_Events;
};