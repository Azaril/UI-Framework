#pragma once

#include "TesselationSink.h"
#include "OpenGLES20VertexBuffer.h"

class COpenGLES20RenderTarget;

struct ITesselationBatchCallback
{
    virtual __checkReturn HRESULT OnTesselatedGeometryBatch(
        __in COpenGLES20VertexBuffer* pBuffer
        ) = 0;
};

class COpenGLES20TesselationSink : public CRefCountedObjectBase< ITesselationSink >
{
    public:
        DECLARE_FACTORY3( COpenGLES20TesselationSink, ITesselationBatchCallback*, COpenGLES20VertexBuffer**, UINT32 );
    
	    __override virtual __checkReturn HRESULT AddTriangle(
	        const Point2F& point1,
	        const Point2F& point2,
	        const Point2F& point3
	        );    	
    
        __checkReturn HRESULT Flush(
            );
    
        __checkReturn HRESULT SetDiffuseColor(
            const ColorF& Color
            );
    
        __checkReturn HRESULT SetTransform(
            const Matrix3X2F& Transform
            );

    protected:    
        COpenGLES20TesselationSink(
           );
    
        virtual ~COpenGLES20TesselationSink(
            );    
    
        __checkReturn HRESULT Initialize(
	        __in ITesselationBatchCallback* pCallback,
	        __in_ecount(VertexBufferCount) COpenGLES20VertexBuffer** ppVertexBuffers,
	        UINT32 VertexBufferCount
	        );
    
        __checkReturn HRESULT FlushVertexCache(
            );
    
        size_t GetAvailableVertexBufferCount(
            );
    
        size_t GetUsedVertexBufferCount(
            );    

    	ITesselationBatchCallback* m_pCallback;
        COpenGLES20VertexBuffer** m_ppVertexBuffers;
        UINT32 m_VertexBufferCount;
        UINT32 m_CurrentVertexBuffer;
        RenderVertex m_VertexCache[2048];
        RenderVertex* m_pCacheWriteOffset;    
        ColorF m_DiffuseColor;
        Matrix3X2F m_Transform;
};