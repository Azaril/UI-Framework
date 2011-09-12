#pragma once

#include "RefCounted.h"
#include "RectangleGraphicsGeometry.h"
#include "RoundedRectangleGraphicsGeometry.h"
#include "GraphicsGeometryBuilder.h"

class CGeometryProvider : public CRefCountedObject
{
    public:
        virtual __checkReturn HRESULT CreateRectangleGeometry( 
			const RectF& Rectangle, 
			__deref_out CRectangleGraphicsGeometry** ppRectangleGeometry 
			) = 0;

        virtual __checkReturn HRESULT CreateRoundedRectangleGeometry( 
			const RectF& Rectangle, 
			FLOAT CornerRadius, 
			__deref_out CRoundedRectangleGraphicsGeometry** ppRectangleGeometry 
			) = 0;

        virtual __checkReturn HRESULT CreateGeometryBuilder(
            __deref_out CGraphicsGeometryBuilder** ppGeometryBuilder
            ) = 0;
};

