#include "RoutedCommandEventArgs.h"

CRoutedCommandEventArgs::CRoutedCommandEventArgs() : m_Command(NULL)
{
}

CRoutedCommandEventArgs::~CRoutedCommandEventArgs()
{
    ReleaseObject(m_Command);
}

HRESULT CRoutedCommandEventArgs::Initialize(CRoutedEvent* pRoutedEvent, CRoutedCommand* pCommand)
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

CRoutedCommand* CRoutedCommandEventArgs::GetCommand()
{
    return m_Command;
}