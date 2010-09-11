#include "InputEventArgs.h"

//
// CInputEventArgs
//
extern "C" __declspec(dllexport)
TypeIndex::Value CInputEventArgs_TypeIndex()
{
    return TypeIndex::InputEventArgs;
}

extern "C" __declspec(dllexport)
CRoutedEventArgs* CInputEventArgs_CastTo_CRoutedEventArgs(CInputEventArgs* pArgs)
{
    return pArgs;
}

extern "C" __declspec(dllexport)
CInputEventArgs* CObjectWithType_CastTo_CInputEventArgs(CObjectWithType* pObject)
{
    return (pObject->IsTypeOf(TypeIndex::InputEventArgs)) ? (CInputEventArgs*)pObject : NULL;
}