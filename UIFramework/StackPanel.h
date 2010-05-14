#pragma once

#include "Panel.h"

namespace Orientation
{
    enum Value
    {
        Vertical,
        Horizontal
    };
}

class CStackPanel : public CPanel
{
    public:
        DECLARE_FACTORY1( CStackPanel, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::StackPanel, CPanel );

        //static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        HRESULT SetOrientation( Orientation::Value Direction );

    public:
        CStackPanel();
        virtual ~CStackPanel();

        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
        virtual HRESULT ArrangeInternal( SizeF Size );

        Orientation::Value m_Orientation;
};

template< >
struct ObjectTypeTraits< CStackPanel >
{
    static const TypeIndex::Value Type = TypeIndex::StackPanel;
};