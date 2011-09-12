#pragma once

#include "GraphicsGeometry.h"

class UIFRAMEWORK_API CRectangleGraphicsGeometry : public CGraphicsGeometry
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::RectangleGraphicsGeometry, CGraphicsGeometry );
};

template< >
struct ObjectTypeTraits< CRectangleGraphicsGeometry >
{
    static const TypeIndex::Value Type = TypeIndex::RectangleGraphicsGeometry;
};