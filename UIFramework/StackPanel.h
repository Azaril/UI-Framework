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

        virtual HRESULT Arrange( SizeF Size );

        HRESULT SetOrientation( Orientation::Value Direction );

    public:
        CStackPanel();
        virtual ~CStackPanel();

        HRESULT Initialize();

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );

        Orientation::Value m_Orientation;
};
