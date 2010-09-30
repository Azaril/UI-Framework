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

        HRESULT GetEffectiveMaximum( FLOAT* pMaximum );
        HRESULT GetEffectiveMinimum( FLOAT* pMinimum );
        HRESULT GetEffectiveRangeValue( FLOAT* pValue );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnMaximumChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnMinimumChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnValueChanged );

        HRESULT OnMaximumChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnMinimumChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnValueChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        CTypedLayeredValue< CRangeBase, CFloatValue > m_Maximum;
        CTypedLayeredValue< CRangeBase, CFloatValue > m_Minimum;
        CTypedLayeredValue< CRangeBase, CFloatValue > m_Value;
};

template< >
struct ObjectTypeTraits< CRangeBase >
{
    static const TypeIndex::Value Type = TypeIndex::RangeBase;
};