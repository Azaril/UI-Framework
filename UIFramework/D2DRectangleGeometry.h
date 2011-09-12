#pragma once

#include "GraphicsGeometry.h"
#include "Factory.h"
#include "RectangleGraphicsGeometry.h"
#include "D2DGeometryBase.h"

class CD2DRectangleGeometry : public CD2DGeometryBase< CRefCountedObjectBase< CRectangleGraphicsGeometry > >
{
    public:
        DECLARE_FACTORY1( CD2DRectangleGeometry, ID2D1RectangleGeometry* );

    protected:
        CD2DRectangleGeometry(
			);

        virtual ~CD2DRectangleGeometry(
			);

        __checkReturn HRESULT Initialize( 
			__in ID2D1RectangleGeometry* pRectangleGeometry 
			);

        ID2D1RectangleGeometry* m_RectangleGeometry;
};
