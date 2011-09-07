#pragma once

#include "Command.h"
#include "StaticRoutedEvent.h"

class CRoutedCommand : public CCommand
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::RoutedCommand, CCommand );

        static __checkReturn HRESULT CreateEventInformation( 
            __deref_out CEventInformation** ppInformation 
            );
        
        __override virtual __checkReturn HRESULT Execute(
            __in CUIElement* pSender 
            );

        static CStaticRoutedEvent ExecutedEvent;

    protected:
        CRoutedCommand(
            );

        virtual ~CRoutedCommand(
            );
};

template< >
struct ObjectTypeTraits< CRoutedCommand >
{
    static const TypeIndex::Value Type = TypeIndex::RoutedCommand;
};