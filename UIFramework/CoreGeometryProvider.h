#pragma once

#include "Factory.h"
#include "GeometryProvider.h"
#include "GeometryBuilder.h"
#include "GraphicsGeometryBuilder.h"

class CCoreGeometryProvider : public CGeometryProvider
{
    public:
        DECLARE_FACTORY( CCoreGeometryProvider );

        __override virtual __checkReturn HRESULT CreateRectangleGeometry( 
			const RectF& Rectangle, 
			__deref_out CRectangleGraphicsGeometry** ppRectangleGeometry 
			);

        __override virtual __checkReturn HRESULT CreateRoundedRectangleGeometry( 
			const RectF& Rectangle, 
			FLOAT CornerRadius, 
			__deref_out CRoundedRectangleGraphicsGeometry** ppRectangleGeometry 
			);

        __override virtual __checkReturn HRESULT CreateGeometryBuilder(
            __deref_out CGraphicsGeometryBuilder** ppGeometryBuilder
            );

    protected:
        CCoreGeometryProvider(
            );

        virtual ~CCoreGeometryProvider(
            );

        __checkReturn HRESULT Initialize(
            );
};

