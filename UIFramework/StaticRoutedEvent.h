#pragma once

#include "RoutedEvent.h"

class UIFRAMEWORK_API CStaticRoutedEvent : public CRoutedEvent
{
    public:
        CStaticRoutedEvent( 
            __in const WCHAR* pName, 
            RoutingStrategy::Value Strategy 
            );

        STATIC_REFCOUNTING();

        __override virtual RoutingStrategy::Value GetRoutingStrategy(
            );

        __override virtual __out const WCHAR* GetName(
            );

    protected:
        const WCHAR* m_Name;
        RoutingStrategy::Value m_Strategy;
};
