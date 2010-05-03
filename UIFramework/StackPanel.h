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
        DECLARE_FACTORY( CStackPanel );

        DECLARE_TYPE_WITH_BASE( TypeIndex::StackPanel, CPanel );

        HRESULT SetOrientation( Orientation::Value Direction );

    public:
        CStackPanel();
        virtual ~CStackPanel();

        HRESULT Initialize();

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
        virtual HRESULT ArrangeInternal( SizeF Size );

        //virtual HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        Orientation::Value m_Orientation;
};

template< >
struct ObjectTypeTraits< CStackPanel >
{
    static const TypeIndex::Value Type = TypeIndex::StackPanel;
};