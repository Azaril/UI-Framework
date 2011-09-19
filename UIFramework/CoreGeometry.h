#pragma once

#include "RefCounted.h"
#include "PathGraphicsGeometry.h"
#include "CoreShapeData.h"
#include "TesselationSink.h"

template< typename T >
class CCoreGeometry : public CRefCountedObjectBase< T >
{
    public:
        virtual __checkReturn HRESULT TesselateFill(
            __in ITesselationSink* pSink
            ) = 0;    
};

