#include "InputEventArgs.h"

//
// CInputEventArgs
//
extern "C" __declspec(dllexport)
TypeIndex::Value 
CInputEventArgs_TypeIndex(
    )
{
    return TypeIndex::InputEventArgs;
}

extern "C" __declspec(dllexport)
__out CRoutedEventArgs* 
CInputEventArgs_CastTo_CRoutedEventArgs(
    __in CInputEventArgs* pArgs
    )
{
    return pArgs;
}

extern "C" __declspec(dllexport)
__out_opt CInputEventArgs*
CObjectWithType_CastTo_CInputEventArgs(
    __in CObjectWithType* pObject
    )
{
    return (pObject->IsTypeOf(TypeIndex::InputEventArgs)) ? (CInputEventArgs*)pObject : NULL;
}