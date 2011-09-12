#pragma once

#include "ShapeBuilder.h"
#include "GraphicsGeometryBuilder.h"
#include "CorePathGraphicsGeometry.h"

class CCoreGraphicsShapeBuilder : public CShapeBuilder< CGraphicsGeometryBuilder >
{
    public:
        DECLARE_FACTORY( CCoreGraphicsShapeBuilder );

        __override virtual __checkReturn HRESULT GetGraphicsGeometry(
            __deref_out CGraphicsGeometry** ppGeometry
            );

    protected:
        CCoreGraphicsShapeBuilder(
            );

        virtual ~CCoreGraphicsShapeBuilder(
            );

        __checkReturn HRESULT Initialize(
            );

        CCorePathGraphicsGeometry* m_pPathGeometry;
};

