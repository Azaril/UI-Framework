#include "StaticRoutedCommand.h"

CStaticRoutedCommand::CStaticRoutedCommand(const WCHAR* pName) : m_Name(pName)
{
}

CStaticRoutedCommand::~CStaticRoutedCommand()
{
}

INT32 CStaticRoutedCommand::AddRef()
{
    return 1;
}

INT32 CStaticRoutedCommand::Release()
{
    return 1;
}

const WCHAR* CStaticRoutedCommand::GetName()
{
    return m_Name;
}