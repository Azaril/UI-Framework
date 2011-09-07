#include "StaticRoutedEvent.h"

CStaticRoutedEvent::CStaticRoutedEvent(
    __in const WCHAR* pName, 
    RoutingStrategy::Value Strategy
    ) 
    : m_Name(pName)
    , m_Strategy(Strategy)
{
}

__override INT32 
CStaticRoutedEvent::AddRef(
    )
{
    return 1;
}

__override INT32 
CStaticRoutedEvent::Release(
    )
{
    return 1;
}

__override RoutingStrategy::Value 
CStaticRoutedEvent::GetRoutingStrategy(
    )
{
    return m_Strategy;
}

__override __out const WCHAR*
CStaticRoutedEvent::GetName(
    )
{
    return m_Name;
}