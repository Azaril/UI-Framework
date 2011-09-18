#include "StaticTesselator.h"
#include "ErrorChecking.h"

namespace StaticTesselator
{
    
__checkReturn HRESULT TesselateRectangle(
    const RectF& Rect,
    const Matrix3X2F& Transform,
    __out_ecount(VertexBufferSize) RenderVertex* pVertices,
    UINT32 VertexBufferSize,
    __out UINT32* pVerticesWritten
    )
{
    HRESULT hr = S_OK;
    RenderVertex* pWriteOffset = NULL;
    
    IFCEXPECT(VertexBufferSize >= VerticesNeededForRectangleTesselation);
    
    {
        //TODO: Remove extra redundant transforms.
        Point2F TopLeft = Transform.TransformPoint(Point2F(Rect.left, Rect.top));
        Point2F TopRight = Transform.TransformPoint(Point2F(Rect.right, Rect.top));
        Point2F BottomLeft = Transform.TransformPoint(Point2F(Rect.left, Rect.bottom));
        Point2F BottomRight = Transform.TransformPoint(Point2F(Rect.right, Rect.bottom));        
        
        pWriteOffset = pVertices;
        
        pWriteOffset->Position = TopLeft;
        
        ++pWriteOffset;
        
        pWriteOffset->Position = TopRight;    
        
        ++pWriteOffset;
        
        pWriteOffset->Position = BottomLeft;
        
        ++pWriteOffset; 
        
        pWriteOffset->Position = BottomLeft;
        
        ++pWriteOffset;         
        
        pWriteOffset->Position = TopRight;
        
        ++pWriteOffset;
        
        pWriteOffset->Position = BottomRight;
        
        ++pWriteOffset;    
        
        *pVerticesWritten = VerticesNeededForRectangleTesselation;
    }
    
Cleanup:
    return hr;
}
    
}