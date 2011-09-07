#pragma once

#include "Enums.h"
#include "InputEventArgs.h"

class CKeyboardEventArgs : public CInputEventArgs
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::KeyboardEventArgs, CInputEventArgs );
};

template< >
struct ObjectTypeTraits< CKeyboardEventArgs >
{
    static const TypeIndex::Value Type = TypeIndex::KeyboardEventArgs;
};

class CKeyEventArgs : public CKeyboardEventArgs
{
    public:
        DECLARE_FACTORY3( CKeyEventArgs, CRoutedEvent*, Key::Value, KeyState::Value );
        DECLARE_FACTORY2( CKeyEventArgs, CRoutedEvent*, CKeyEventArgs* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::KeyEventArgs, CKeyboardEventArgs );

        Key::Value GetKey(
            );

        KeyState::Value GetState(
            );

    protected:
        CKeyEventArgs(
            );

        virtual ~CKeyEventArgs(
            );

        __checkReturn HRESULT Initialize(
            __in CRoutedEvent* pRoutedEvent, 
            Key::Value Key,
            KeyState::Value State 
            );

        __checkReturn HRESULT Initialize(
            __in CRoutedEvent* pRoutedEvent,
            __in CKeyEventArgs* pSourceArgs 
            );

        Key::Value m_Key;
        KeyState::Value m_State;
};

template< >
struct ObjectTypeTraits< CKeyEventArgs >
{
    static const TypeIndex::Value Type = TypeIndex::KeyEventArgs;
};