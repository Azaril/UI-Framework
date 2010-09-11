#include "KeyboardEventArgs.h"

CKeyEventArgs::CKeyEventArgs() : m_Key((Key::Value)0),
                                 m_State(KeyState::Up)
{
}

CKeyEventArgs::~CKeyEventArgs()
{
}

HRESULT CKeyEventArgs::Initialize(CRoutedEvent* pRoutedEvent, Key::Value Key, KeyState::Value State)
{
    HRESULT hr = S_OK;

    IFCPTR(pRoutedEvent);
    
    IFC(CInputEventArgs::Initialize(pRoutedEvent));

    m_Key = Key;
    m_State = State;

Cleanup:
    return hr;
}

HRESULT CKeyEventArgs::Initialize(CRoutedEvent* pRoutedEvent, CKeyEventArgs* pSourceArgs)
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

Key::Value CKeyEventArgs::GetKey()
{
    return m_Key;
}

KeyState::Value CKeyEventArgs::GetState()
{
    return m_State;
}

//
// CKeyboardEventArgs
//
extern "C" __declspec(dllexport)
TypeIndex::Value CKeyboardEventArgs_TypeIndex()
{
    return TypeIndex::KeyboardEventArgs;
}

extern "C" __declspec(dllexport)
CInputEventArgs* CKeyboardEventArgs_CastTo_CInputEventArgs(CKeyboardEventArgs* pArgs)
{
    return pArgs;
}

extern "C" __declspec(dllexport)
CKeyboardEventArgs* CObjectWithType_CastTo_CKeyboardEventArgs(CObjectWithType* pObject)
{
    return (pObject->IsTypeOf(TypeIndex::KeyboardEventArgs)) ? (CKeyboardEventArgs*)pObject : NULL;
}

//
// CKeyEventArgs
//
extern "C" __declspec(dllexport)
TypeIndex::Value CKeyEventArgs_TypeIndex()
{
    return TypeIndex::KeyEventArgs;
}

extern "C" __declspec(dllexport)
CKeyboardEventArgs* CKeyEventArgs_CastTo_CKeyboardEventArgs(CKeyEventArgs* pArgs)
{
    return pArgs;
}

extern "C" __declspec(dllexport)
CKeyEventArgs* CObjectWithType_CastTo_CKeyEventArgs(CObjectWithType* pObject)
{
    return (pObject->IsTypeOf(TypeIndex::KeyEventArgs)) ? (CKeyEventArgs*)pObject : NULL;
}

extern "C" __declspec(dllexport)
Key::Value CKeyEventArgs_GetKey(CKeyEventArgs* pArgs)
{
    return pArgs->GetKey();
}

extern "C" __declspec(dllexport)
KeyState::Value CKeyEventArgs_GetState(CKeyEventArgs* pArgs)
{
    return pArgs->GetState();
}