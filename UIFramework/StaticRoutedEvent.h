#pragma once

#include "RoutedEvent.h"

class UIFRAMEWORK_API CStaticRoutedEvent : public CRoutedEvent
{
    public:
        CStaticRoutedEvent( 
            __in const WCHAR* pName, 
            RoutingStrategy::Value Strategy 
            );

        __override virtual INT32 AddRef(
            );

        __override virtual INT32 Release(
            );

        __override virtual RoutingStrategy::Value GetRoutingStrategy(
            );

        __override virtual __out const WCHAR* GetName(
            );

    protected:
        const WCHAR* m_Name;
        RoutingStrategy::Value m_Strategy;
};
