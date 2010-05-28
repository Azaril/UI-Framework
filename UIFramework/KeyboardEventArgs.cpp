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

UINT32 CKeyEventArgs::GetKey()
{
    return m_Key;
}

KeyState::Value CKeyEventArgs::GetState()
{
    return m_State;
}