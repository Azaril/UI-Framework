#pragma once

#include "GraphicsGeometry.h"

class UIFRAMEWORK_API CRoundedRectangleGraphicsGeometry : public CGraphicsGeometry
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::RoundedRectangleGraphicsGeometry, CGraphicsGeometry );
};

template< >
struct ObjectTypeTraits< CRoundedRectangleGraphicsGeometry >
{
    static const TypeIndex::Value Type = TypeIndex::RoundedRectangleGraphicsGeometry;
};
