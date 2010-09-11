#include "RoutedEvent.h"

//
// CRoutedEvent
//
extern "C" __declspec(dllexport)
TypeIndex::Value CRoutedEvent_TypeIndex()
{
    return TypeIndex::RoutedEvent;
}

extern "C" __declspec(dllexport)
CObjectWithType* CRoutedEvent_CastTo_CObjectWithType(CRoutedEvent* pEvent)
{
    return pEvent;
}

extern "C" __declspec(dllexport)
CRoutedEvent* CObjectWithType_CastTo_CRoutedEvent(CObjectWithType* pObject)
{
    return (pObject->IsTypeOf(TypeIndex::RoutedEvent)) ? (CRoutedEvent*)pObject : NULL;
}