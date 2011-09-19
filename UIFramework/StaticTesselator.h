#pragma once

#include "Types.h"
#include "RenderVertex.h"
#include "TesselationSink.h"

namespace StaticTesselator
{    
    __checkReturn HRESULT TesselateRectangle(
        const RectF& Rect,
        __in ITesselationSink* pSink                                             
        );    
    
    __checkReturn HRESULT TesselateRectangle(
        const RectF& Rect,
        const Matrix3X2F& Transform,
        __in ITesselationSink* pSink                                             
        );
};