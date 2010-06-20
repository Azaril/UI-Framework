#pragma once

#include "PropertyObject.h"
#include "Factory.h"

class CDelegatingRoutedEventInformation : public CEventInformation
{
    public:
        DECLARE_FACTORY2( CDelegatingRoutedEventInformation, CEventInformation*, CEventInformation* );

        virtual HRESULT GetEvent( const WCHAR* pEventName, CRoutedEvent** ppEvent );

    protected:
        CDelegatingRoutedEventInformation();
        virtual ~CDelegatingRoutedEventInformation();

        HRESULT Initialize( CEventInformation* pRoot, CEventInformation* pBase );

        CEventInformation* m_Root;
        CEventInformation* m_Base;
};