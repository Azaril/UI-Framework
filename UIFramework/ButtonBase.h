#pragma once

#include "ContentControl.h"
#include "Command.h"

class CButtonBase : public CContentControl
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::ButtonBase, CContentControl );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );
        static HRESULT CreateEventInformation( CEventInformation** ppInformation );

        //
        // Properties
        //
        static CStaticProperty CommandProperty;

        //
        // Events
        //
        static CStaticRoutedEvent ClickEvent;
        
    protected:
        CButtonBase();
        virtual ~CButtonBase();

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        HRESULT RaiseClick();

        virtual void OnMouseLeftButtonDown( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );
        virtual void OnMouseLeftButtonUp( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );

        HRESULT GetEffectiveCommand( CCommand** ppCommand );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnCommandChanged );

        HRESULT OnCommandChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        CTypedLayeredValue< CButtonBase, CCommand > m_Command;
};

template< >
struct ObjectTypeTraits< CButtonBase >
{
    static const TypeIndex::Value Type = TypeIndex::ButtonBase;
};
