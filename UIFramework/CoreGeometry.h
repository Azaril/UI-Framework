#pragma once

#include "RefCounted.h"
#include "PathGraphicsGeometry.h"
#include "CoreShapeData.h"
#include "TesselationSink.h"

struct ICoreGeometry
{
    virtual __checkReturn HRESULT TesselateFill(
        __in ITesselationSink* pSink
        ) = 0;       
};

template< typename T >
class CCoreGeometry : public CRefCountedObjectBase< T >,
                      public ICoreGeometry
{
    public: 
};

