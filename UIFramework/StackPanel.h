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

        virtual TypeIndex::Value GetType() { return TypeIndex::StackPanel; }
        virtual BOOL IsTypeOf( TypeIndex::Value Type ) { return Type == TypeIndex::Panel || CFrameworkElement::IsTypeOf(Type); }

        virtual HRESULT Arrange( SizeF Size );

        HRESULT SetOrientation( Orientation::Value Direction );

    public:
        CStackPanel();
        virtual ~CStackPanel();

        HRESULT Initialize();

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );

        //virtual HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        Orientation::Value m_Orientation;
};
