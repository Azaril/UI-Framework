#include "KeyboardEventArgs.h"

CKeyEventArgs::CKeyEventArgs() : m_Key(0),
                                 m_State(KeyState::Up)
{
}

CKeyEventArgs::~CKeyEventArgs()
{
}

HRESULT CKeyEventArgs::Initialize(CRoutedEvent* pRoutedEvent, UINT32 Key, KeyState::Value State)
{
    HRESULT hr = S_OK;

    IFCPTR(pRoutedEvent);
    
    IFC(CInputEventArgs::Initialize(pRoutedEvent));

    m_Key = Key;
    m_State = State;

Cleanup:
    return hr;
}

UINT32 CKeyEventArgs::GetKey()
{
    return m_Key;
}

KeyState::Value CKeyEventArgs::GetState()
{
    return m_State;
}