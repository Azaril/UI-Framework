#pragma once

#include "Command.h"
#include "StaticRoutedEvent.h"

class CRoutedCommand : public CCommand
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::RoutedCommand, CCommand );

        static HRESULT CreateEventInformation( CEventInformation** ppInformation );
        
        virtual HRESULT Execute( CUIElement* pSender );

        static CStaticRoutedEvent< RoutingStrategy::Bubbling > ExecutedEvent;

    protected:
        CRoutedCommand();
        virtual ~CRoutedCommand();
};

template< >
struct ObjectTypeTraits< CRoutedCommand >
{
    static const TypeIndex::Value Type = TypeIndex::RoutedCommand;
};