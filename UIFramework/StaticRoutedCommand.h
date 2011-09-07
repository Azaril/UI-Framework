#pragma once

#include "RoutedCommand.h"

class CStaticRoutedCommand : public CRoutedCommand
{
    public:
        CStaticRoutedCommand( 
            __in const WCHAR* pName 
            );

        virtual ~CStaticRoutedCommand(
            );

        __override virtual INT32 AddRef(
            );

        __override virtual INT32 Release(
            );

        __override virtual __out const WCHAR* GetName(
            );

    protected:
        const WCHAR* m_Name;
};