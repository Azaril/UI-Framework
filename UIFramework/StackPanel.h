#pragma once

#include "Panel.h"

class UIFRAMEWORK_API CStackPanel : public CPanel
{
    public:
        DECLARE_FACTORY1( CStackPanel, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::StackPanel, CPanel );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        //
        // Properties
        //
        static CStaticProperty OrientationProperty;

    public:
        CStackPanel();
        virtual ~CStackPanel();

        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
        virtual HRESULT ArrangeInternal( SizeF AvailableSize, SizeF& UsedSize );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        HRESULT GetEffectiveOrientation( Orientation::Value* pOrientation );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnOrientationChanged );

        HRESULT OnOrientationChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        CTypedLayeredValue< COrientationValue > m_Orientation;
};

template< >
struct ObjectTypeTraits< CStackPanel >
{
    static const TypeIndex::Value Type = TypeIndex::StackPanel;
};