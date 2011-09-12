#pragma once

#include "GraphicsGeometry.h"

class UIFRAMEWORK_API CPathGraphicsGeometry : public CGraphicsGeometry
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::PathGraphicsGeometry, CGraphicsGeometry );
};

template< >
struct ObjectTypeTraits< CPathGraphicsGeometry >
{
    static const TypeIndex::Value Type = TypeIndex::PathGraphicsGeometry;
};