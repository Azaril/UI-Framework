#pragma once

#include "RoutedCommand.h"

class CStaticRoutedCommand : public CRoutedCommand
{
    public:
        CStaticRoutedCommand( const WCHAR* pName );
        virtual ~CStaticRoutedCommand();

        virtual INT32 AddRef();
        virtual INT32 Release();

        virtual const WCHAR* GetName();

    protected:
        const WCHAR* m_Name;
};