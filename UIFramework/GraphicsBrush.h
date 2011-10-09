#pragma once

#include "RefCounted.h"

class CGraphicsBrush : public CRefCountedObject
{
    public:
        virtual __checkReturn HRESULT GetBounds(
            RectF& brushBounds
            ) = 0;

        virtual __checkReturn HRESULT SetTransform( 
			const Matrix3X2F& Transform 
			) = 0;
    
        virtual void GetTransform(
            Matrix3X2F& Transform
            ) const = 0;
};
