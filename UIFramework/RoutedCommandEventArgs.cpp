#include "RoutedCommandEventArgs.h"

CRoutedCommandEventArgs::CRoutedCommandEventArgs(
    ) 
    : m_Command(NULL)
{
}

CRoutedCommandEventArgs::~CRoutedCommandEventArgs(
    )
{
    ReleaseObject(m_Command);
}

__checkReturn HRESULT 
CRoutedCommandEventArgs::Initialize(
    __in CRoutedEvent* pRoutedEvent,
    __in CRoutedCommand* pCommand
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pRoutedEvent);
    IFCPTR(pCommand);

    IFC(CRoutedEventArgs::Initialize(pRoutedEvent));

    m_Command = pCommand;
    AddRefObject(m_Command);

Cleanup:
    return hr;
}

__out CRoutedCommand* 
CRoutedCommandEventArgs::GetCommand(
    )
{
    return m_Command;
}