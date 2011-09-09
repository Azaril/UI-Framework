#include "RoutedEvent.h"

//
// CRoutedEvent
//
extern "C" UIFRAMEWORK_API
TypeIndex::Value 
RoutedEvent_TypeIndex(
    )
{
    return TypeIndex::RoutedEvent;
}

extern "C" UIFRAMEWORK_API
__out CObjectWithType* 
CRoutedEvent_CastTo_CObjectWithType(
    __in CRoutedEvent* pEvent
    )
{
    return pEvent;
}

extern "C" UIFRAMEWORK_API
__out_opt CRoutedEvent* 
CObjectWithType_CastTo_CRoutedEvent(
    __in CObjectWithType* pObject
    )
{
    return (pObject->IsTypeOf(TypeIndex::RoutedEvent)) ? (CRoutedEvent*)pObject : NULL;
}