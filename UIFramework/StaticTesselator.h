#pragma once

#include "Types.h"
#include "RenderVertex.h"
#include "TesselationSink.h"

namespace StaticTesselator
{    
    __checkReturn HRESULT TesselateRectangle(
        const RectF& rect,
        __in ITesselationSink* pSink                                             
        );    
    
    __checkReturn HRESULT TesselateRectangle(
        const RectF& rect,
        const Matrix3X2F& transform,
        __in ITesselationSink* pSink                                             
        );

    __checkReturn HRESULT TesselateRectangleStroke(
    	const RectF& rect,
    	FLOAT strokeWidth,
    	__in ITesselationSink* pSink
    	);

    __checkReturn HRESULT TesselateRectangleStroke(
    	const RectF& rect,
    	FLOAT strokeWidth,
    	const Matrix3X2F& transform,
    	__in ITesselationSink* pSink
    	);    	
};