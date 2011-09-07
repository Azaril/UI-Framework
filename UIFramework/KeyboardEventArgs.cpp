#include "KeyboardEventArgs.h"

CKeyEventArgs::CKeyEventArgs(
    ) 
    : m_Key((Key::Value)0)
    , m_State(KeyState::Up)
{
}

CKeyEventArgs::~CKeyEventArgs(
    )
{
}

__checkReturn HRESULT
CKeyEventArgs::Initialize(
    __in CRoutedEvent* pRoutedEvent, 
    Key::Value Key,
    KeyState::Value State
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pRoutedEvent);
    
    IFC(CInputEventArgs::Initialize(pRoutedEvent));

    m_Key = Key;
    m_State = State;

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CKeyEventArgs::Initialize(
    __in CRoutedEvent* pRoutedEvent, 
    __in CKeyEventArgs* pSourceArgs
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pRoutedEvent);
    IFCPTR(pSourceArgs);

    IFC(CInputEventArgs::Initialize(pRoutedEvent));

    m_Key = pSourceArgs->GetKey();
    m_State = pSourceArgs->GetState();

Cleanup:
    return hr;
}

Key::Value 
CKeyEventArgs::GetKey(
    )
{
    return m_Key;
}

KeyState::Value 
CKeyEventArgs::GetState(
    )
{
    return m_State;
}

//
// CKeyboardEventArgs
//
extern "C" __declspec(dllexport)
TypeIndex::Value 
CKeyboardEventArgs_TypeIndex()
{
    return TypeIndex::KeyboardEventArgs;
}

extern "C" __declspec(dllexport)
__out CInputEventArgs* 
CKeyboardEventArgs_CastTo_CInputEventArgs(
    __in CKeyboardEventArgs* pArgs
    )
{
    return pArgs;
}

extern "C" __declspec(dllexport)
__out_opt CKeyboardEventArgs* 
CObjectWithType_CastTo_CKeyboardEventArgs(
    __in CObjectWithType* pObject
    )
{
    return (pObject->IsTypeOf(TypeIndex::KeyboardEventArgs)) ? (CKeyboardEventArgs*)pObject : NULL;
}

//
// CKeyEventArgs
//
extern "C" __declspec(dllexport)
TypeIndex::Value
CKeyEventArgs_TypeIndex()
{
    return TypeIndex::KeyEventArgs;
}

extern "C" __declspec(dllexport)
__out CKeyboardEventArgs*
CKeyEventArgs_CastTo_CKeyboardEventArgs(
    __in CKeyEventArgs* pArgs
    )
{
    return pArgs;
}

extern "C" __declspec(dllexport)
__out_opt CKeyEventArgs* 
CObjectWithType_CastTo_CKeyEventArgs(
    __in CObjectWithType* pObject
    )
{
    return (pObject->IsTypeOf(TypeIndex::KeyEventArgs)) ? (CKeyEventArgs*)pObject : NULL;
}

extern "C" __declspec(dllexport)
Key::Value 
CKeyEventArgs_GetKey(
    __in CKeyEventArgs* pArgs
    )
{
    return pArgs->GetKey();
}

extern "C" __declspec(dllexport)
KeyState::Value 
CKeyEventArgs_GetState(
    __in CKeyEventArgs* pArgs
    )
{
    return pArgs->GetState();
}