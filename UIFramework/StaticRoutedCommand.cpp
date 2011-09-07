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

__override INT32 
CStaticRoutedCommand::AddRef(
    )
{
    return 1;
}

__override INT32 
CStaticRoutedCommand::Release(
    )
{
    return 1;
}

__override __out const WCHAR* 
CStaticRoutedCommand::GetName(
    )
{
    return m_Name;
}