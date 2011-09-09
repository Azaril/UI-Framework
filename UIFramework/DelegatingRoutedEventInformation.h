#pragma once

#include "PropertyObject.h"
#include "Factory.h"

class CDelegatingRoutedEventInformation : public CEventInformation
{
    public:
        DECLARE_FACTORY2( CDelegatingRoutedEventInformation, CEventInformation*, CEventInformation* );

        __override virtual __checkReturn HRESULT GetEvent(
            __in_z const WCHAR* pEventName, 
            __deref_out_opt CRoutedEvent** ppEvent 
            );

    protected:
        CDelegatingRoutedEventInformation(
            );

        virtual ~CDelegatingRoutedEventInformation(
            );

        __checkReturn HRESULT Initialize(
            __in CEventInformation* pRoot,
            __in CEventInformation* pBase 
            );

        CEventInformation* m_Root;
        CEventInformation* m_Base;
};