#pragma once

#include "Panel.h"

class CCanvas : public CPanel
{
    public:
        DECLARE_FACTORY( CCanvas );

        DECLARE_TYPE_WITH_BASE( TypeIndex::Canvas, CPanel );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult );

    public:
        CCanvas();
        virtual ~CCanvas();

        HRESULT Initialize();

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
        virtual HRESULT ArrangeInternal( SizeF Size );
};

template< >
struct ObjectTypeTraits< CCanvas >
{
    static const TypeIndex::Value Type = TypeIndex::Canvas;
};