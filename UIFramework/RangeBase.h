#pragma once

#include "Control.h"
#include "StaticRoutedCommand.h"
#include "StaticRoutedEvent.h"

class CRangeBase : public CControl
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::RangeBase, CControl );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );
        static HRESULT CreateEventInformation( CEventInformation** ppInformation );

        //
        // Properties
        //
        static CStaticProperty MaximumProperty;
        static CStaticProperty MinimumProperty;
        static CStaticProperty ValueProperty;

        //
        // Events
        //
        static CStaticRoutedEvent ValueChangedEvent;

    protected:
        CRangeBase();
        virtual ~CRangeBase();

        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnMaximumChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnMinimumChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnValueChanged );

        HRESULT OnMaximumChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnMinimumChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnValueChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        CLayeredValue m_Maximum;
        CLayeredValue m_Minimum;
        CLayeredValue m_Value;
};

template< >
struct ObjectTypeTraits< CRangeBase >
{
    static const TypeIndex::Value Type = TypeIndex::RangeBase;
};