#pragma once

#include "Panel.h"

class UIFRAMEWORK_API CGrid : public CPanel
{
    public:
        DECLARE_FACTORY1( CGrid, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::Grid, CPanel );

        //static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        //
        // Properties
        //
        //static CStaticProperty OrientationProperty;

    public:
        CGrid();
        virtual ~CGrid();

        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
        virtual HRESULT ArrangeInternal( SizeF AvailableSize, SizeF& UsedSize );

        //virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );
};

template< >
struct ObjectTypeTraits< CGrid >
{
    static const TypeIndex::Value Type = TypeIndex::StackPanel;
};