#pragma once

#include "Types.h"
#include "RenderVertex.h"

namespace StaticTesselator
{
    static const UINT32 VerticesNeededForRectangleTesselation = 6;
    
    __checkReturn HRESULT TesselateRectangle(
        const RectF& Rect,
        const Matrix3X2F& Transform,
        __out_ecount(VertexBufferSize) RenderVertex* pVertices,
        UINT32 VertexBufferSize,
        __out UINT32* pVerticesWritten
        );
};