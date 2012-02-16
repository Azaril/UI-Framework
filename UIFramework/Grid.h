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

        __override virtual __checkReturn HRESULT MeasureInternal( 
            const SizeF& AvailableSize,
            SizeF& DesiredSize
            );

        __override virtual __checkReturn HRESULT ArrangeInternal(
            const SizeF& AvailableSize,
            SizeF& UsedSize 
            );

        //virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );
};

template< >
struct ObjectTypeTraits< CGrid >
{
    static const TypeIndex::Value Type = TypeIndex::StackPanel;
};