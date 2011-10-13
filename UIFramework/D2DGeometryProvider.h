#pragma once

#include "Factory.h"
#include "GeometryProvider.h"

#include <d2d1.h>

class CD2DGeometryProvider : public CGeometryProvider
{
    public:
        DECLARE_FACTORY1( CD2DGeometryProvider, ID2D1Factory* );

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
        CD2DGeometryProvider(
            );

        virtual ~CD2DGeometryProvider(
            );

        __checkReturn HRESULT Initialize(
            __in ID2D1Factory* pFactory
            );

        ID2D1Factory* m_pFactory;
};

