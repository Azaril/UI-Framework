#pragma once

#include "GraphicsGeometry.h"
#include "Factory.h"
#include "RoundedRectangleGraphicsGeometry.h"
#include "D2DGeometryBase.h"

class CD2DRoundedRectangleGeometry : public CD2DGeometryBase< CRefCountedObjectBase< CRoundedRectangleGraphicsGeometry > >
{
    public:
        DECLARE_FACTORY1( CD2DRoundedRectangleGeometry, ID2D1RoundedRectangleGeometry* );

    protected:
        CD2DRoundedRectangleGeometry(
			);

        virtual ~CD2DRoundedRectangleGeometry(
			);

        __checkReturn HRESULT Initialize(
			__in ID2D1RoundedRectangleGeometry* pRectangleGeometry 
			);

        ID2D1RoundedRectangleGeometry* m_RoundedRectangleGeometry;
};
