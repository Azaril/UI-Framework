#include "StaticRoutedCommand.h"

CStaticRoutedCommand::CStaticRoutedCommand(
    __in const WCHAR* pName
    ) 
    : m_Name(pName)
{
}

CStaticRoutedCommand::~CStaticRoutedCommand(
    )
{
}

__override __out const WCHAR* 
CStaticRoutedCommand::GetName(
    )
{
    return m_Name;
}