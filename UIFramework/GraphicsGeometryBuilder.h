#pragma once

#include "GeometryBuilder.h"
#include "GraphicsGeometry.h"

class UIFRAMEWORK_API CGraphicsGeometryBuilder : public CGeometryBuilder
{
    public:
        virtual __checkReturn HRESULT GetGraphicsGeometry(
            __deref_out CGraphicsGeometry** ppGeometry
            ) = 0;
};

