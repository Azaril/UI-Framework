#include "RoutedEvent.h"

//
// CRoutedEvent
//
extern "C" __declspec(dllexport) 
TypeIndex::Value 
RoutedEvent_TypeIndex(
    )
{
    return TypeIndex::RoutedEvent;
}

extern "C" __declspec(dllexport)
__out CObjectWithType* 
CRoutedEvent_CastTo_CObjectWithType(
    __in CRoutedEvent* pEvent
    )
{
    return pEvent;
}

extern "C" __declspec(dllexport)
__out_opt CRoutedEvent* 
CObjectWithType_CastTo_CRoutedEvent(
    __in CObjectWithType* pObject
    )
{
    return (pObject->IsTypeOf(TypeIndex::RoutedEvent)) ? (CRoutedEvent*)pObject : NULL;
}