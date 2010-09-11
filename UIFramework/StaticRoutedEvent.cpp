#include "StaticRoutedEvent.h"

CStaticRoutedEvent::CStaticRoutedEvent(const WCHAR* pName, RoutingStrategy::Value Strategy) : m_Name(pName),
                                                                                              m_Strategy(Strategy)
{
}

INT32 CStaticRoutedEvent::AddRef()
{
    return 1;
}

INT32 CStaticRoutedEvent::Release()
{
    return 1;
}

RoutingStrategy::Value CStaticRoutedEvent::GetRoutingStrategy()
{
    return m_Strategy;
}

const WCHAR* CStaticRoutedEvent::GetName()
{
    return m_Name;
}