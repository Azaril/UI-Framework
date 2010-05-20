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
        DECLARE_FACTORY3( CKeyEventArgs, CRoutedEvent*, UINT32, KeyState::Value );

        DECLARE_TYPE_WITH_BASE( TypeIndex::KeyEventArgs, CKeyboardEventArgs );

        UINT32 GetKey();
        KeyState::Value GetState();

    protected:
        CKeyEventArgs();
        virtual ~CKeyEventArgs();

        HRESULT Initialize( CRoutedEvent* pRoutedEvent, UINT32 Key, KeyState::Value State );

        UINT32 m_Key;
        KeyState::Value m_State;
};

template< >
struct ObjectTypeTraits< CKeyEventArgs >
{
    static const TypeIndex::Value Type = TypeIndex::KeyEventArgs;
};