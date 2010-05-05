#pragma once

#include "Geometry.h"
#include "Factory.h"
#include "D2DGeometryBase.h"

class CD2DRoundedRectangleGeometry : public CD2DGeometryBase< CRefCountedObjectBase< CRoundedRectangleGeometry > >
{
    public:
        DECLARE_FACTORY1( CD2DRoundedRectangleGeometry, ID2D1RoundedRectangleGeometry* );

    protected:
        CD2DRoundedRectangleGeometry();
        virtual ~CD2DRoundedRectangleGeometry();

        HRESULT Initialize( ID2D1RoundedRectangleGeometry* pRectangleGeometry );

        ID2D1RoundedRectangleGeometry* m_RoundedRectangleGeometry;
};
