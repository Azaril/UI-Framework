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

        STATIC_REFCOUNTING();

        __override virtual __out const WCHAR* GetName(
            );

    protected:
        const WCHAR* m_Name;
};