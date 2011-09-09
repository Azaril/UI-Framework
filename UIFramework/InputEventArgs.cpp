#include "InputEventArgs.h"

//
// CInputEventArgs
//
extern "C" UIFRAMEWORK_API
TypeIndex::Value 
CInputEventArgs_TypeIndex(
    )
{
    return TypeIndex::InputEventArgs;
}

extern "C" UIFRAMEWORK_API
__out CRoutedEventArgs* 
CInputEventArgs_CastTo_CRoutedEventArgs(
    __in CInputEventArgs* pArgs
    )
{
    return pArgs;
}

extern "C" UIFRAMEWORK_API
__out_opt CInputEventArgs*
CObjectWithType_CastTo_CInputEventArgs(
    __in CObjectWithType* pObject
    )
{
    return (pObject->IsTypeOf(TypeIndex::InputEventArgs)) ? (CInputEventArgs*)pObject : NULL;
}