#pragma once

#include "Types.h"
#include "TesselationSink.h"
#include "VertexBuffer.h"
#include "Factory.h"

struct ITesselationBatchCallback
{
    virtual __checkReturn HRESULT OnTesselatedGeometryBatch(
        __in IVertexBuffer* pBuffer
        ) = 0;
};

class CGeometryTesselationSink : public CRefCountedObjectBase< ITesselationSink >
{
    public:
        DECLARE_FACTORY3( CGeometryTesselationSink, ITesselationBatchCallback*, IVertexBuffer**, UINT32 );
    
	    __override virtual __checkReturn HRESULT AddTriangle(
	        const Point2F& point1,
	        const Point2F& point2,
	        const Point2F& point3
	        );    	

        __override virtual __checkReturn HRESULT AddRectangleWithUnitMask(
            const RectF& rect
            );    	
    
        __checkReturn HRESULT Flush(
            );
    
        __checkReturn HRESULT SetDiffuseColor(
            const ColorF& Color
            );
    
        __checkReturn HRESULT SetTransform(
            const Matrix3X2F& Transform
            );
    
        __checkReturn HRESULT SetBrushTransform(
            __in_opt const Matrix3X2F* pTransform
            );

        __checkReturn HRESULT SetMaskTransform(
            __in_opt const Matrix3X2F* pTransform
            );

    protected:    
        CGeometryTesselationSink(
           );
    
        virtual ~CGeometryTesselationSink(
            );    
    
        __checkReturn HRESULT Initialize(
	        __in ITesselationBatchCallback* pCallback,
	        __in_ecount(VertexBufferCount) IVertexBuffer** ppVertexBuffers,
	        UINT32 VertexBufferCount
	        );
    
        __checkReturn HRESULT FlushVertexCache(
            );
    
        size_t GetAvailableVertexBufferCount(
            );
    
        size_t GetUsedVertexBufferCount(
            );    

        inline void AddTrianglePointInternal(
            const Point2F& point
            );

        inline void AddTrianglePointWithMaskInternal(
            const Point2F& point,
            const Point2F& mask
            );

    	ITesselationBatchCallback* m_pCallback;
        IVertexBuffer** m_ppVertexBuffers;
        UINT32 m_VertexBufferCount;
        UINT32 m_CurrentVertexBuffer;
        RenderVertex* m_pVertexCache;
        UINT32 m_VertexCacheSize;
        RenderVertex* m_pCacheWriteOffset;    
        ColorF m_DiffuseColor;
        Matrix3X2F m_Transform;
        bool m_NeedsBrushTransform;
        Matrix3X2F m_BrushTransform;
        bool m_NeedsMaskTransform;
        Matrix3X2F m_MaskTransform;
};