#pragma once

#include "RefCounted.h"

class CGraphicsBrush : public CRefCountedObject
{
    public:
        virtual HRESULT SetTransform( const Matrix3X2& Transform ) = 0;
};
