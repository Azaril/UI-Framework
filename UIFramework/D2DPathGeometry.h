#pragma once

#include "GraphicsGeometry.h"
#include "Factory.h"
#include "PathGraphicsGeometry.h"
#include "D2DGeometryBase.h"

class CD2DPathGeometry : public CD2DGeometryBase< CRefCountedObjectBase< CPathGraphicsGeometry > >
{
    public:
        DECLARE_FACTORY1( CD2DPathGeometry, ID2D1PathGeometry* );

    protected:
        CD2DPathGeometry(
			);

        virtual ~CD2DPathGeometry(
			);

        __checkReturn HRESULT Initialize( 
			__in ID2D1PathGeometry* pPathGeometry 
			);

        ID2D1PathGeometry* m_PathGeometry;
};
