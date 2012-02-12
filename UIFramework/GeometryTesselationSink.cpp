#include "GeometryTesselationSink.h"
#include "ErrorChecking.h"

CGeometryTesselationSink::CGeometryTesselationSink(
	)
	: m_ppVertexBuffers(NULL)
	, m_VertexBufferCount(0)
	, m_CurrentVertexBuffer(0)
    , m_Transform(Matrix3X2F::Identity())
    , m_BrushTransform(Matrix3X2F::Identity())
    , m_NeedsBrushTransform(FALSE)
    , m_MaskTransform(Matrix3X2F::Identity())
    , m_NeedsMaskTransform(FALSE)
    , m_pVertexCache(NULL)
    , m_VertexCacheSize(0)
    , m_pCacheWriteOffset(NULL)
    , m_HasLocalTransform(false)
    , m_LocalTransform(Matrix3X2F::Identity())
{
}

CGeometryTesselationSink::~CGeometryTesselationSink(
	)
{
	if (m_ppVertexBuffers != NULL)
	{
		for (UINT32 i = 0; i < m_VertexBufferCount; ++i)
		{
			ReleaseObject(m_ppVertexBuffers[i]);
		}
	}

	delete [] m_ppVertexBuffers;
    
    delete [] m_pVertexCache;
}

__checkReturn HRESULT
CGeometryTesselationSink::Initialize(
    __in ITesselationBatchCallback* pCallback,
    __in_ecount(VertexBufferCount) IVertexBuffer** ppVertexBuffers,
    UINT32 VertexBufferCount,
    __in_opt Matrix3X2F* pTransform
	)
{
	HRESULT hr = S_OK;

	m_pCallback = pCallback;

    IFCEXPECT(VertexBufferCount > 0);

	m_ppVertexBuffers = new IVertexBuffer*[VertexBufferCount];
	IFCOOM(m_ppVertexBuffers);

	m_VertexBufferCount = VertexBufferCount;

    //
    // Maximum vertex cache size of 4096.
    //
    m_VertexCacheSize = 4096;

	for (UINT32 i = 0; i < VertexBufferCount; ++i)
	{
		m_ppVertexBuffers[i] = ppVertexBuffers[i];
		AddRefObject(m_ppVertexBuffers[i]);

        m_VertexCacheSize = std::min(m_VertexCacheSize, ppVertexBuffers[i]->GetMaximumVertices());
	}

    m_pVertexCache = new RenderVertex[m_VertexCacheSize];
    IFCOOM(m_pVertexCache);

    m_pCacheWriteOffset = m_pVertexCache;

    if (pTransform != NULL)
    {
        m_HasLocalTransform = true;
        m_LocalTransform = *pTransform;
    }

Cleanup:
	return hr;
}

__checkReturn HRESULT
CGeometryTesselationSink::AddTriangle(
    const Point2F& point1,
    const Point2F& point2,
    const Point2F& point3
	)
{
	HRESULT hr = S_OK;

	if (GetAvailableVertexBufferCount() < 3)
	{
		IFC(FlushVertexCache());
	}

    AddTrianglePointInternal(point1);
    AddTrianglePointInternal(point2);
    AddTrianglePointInternal(point3);

Cleanup:
	return hr;
}

inline void
CGeometryTesselationSink::AddTrianglePointInternal(
    const Point2F& point
    )
{
    m_pCacheWriteOffset->Position = m_Transform.TransformPoint(point);
    m_pCacheWriteOffset->Color = m_DiffuseColor;
    m_pCacheWriteOffset->TextureCoordinates = m_NeedsBrushTransform ? m_BrushTransform.TransformPoint(point) : Point2F(0.0f, 0.0f);
    m_pCacheWriteOffset->MaskCoordinates = m_NeedsMaskTransform ? m_MaskTransform.TransformPoint(point) : Point2F(0.0f, 0.0f);

    ++m_pCacheWriteOffset;
}

inline void
CGeometryTesselationSink::AddTrianglePointWithMaskInternal(
    const Point2F& point,
    const Point2F& mask
    )
{
    m_pCacheWriteOffset->Position = m_Transform.TransformPoint(point);
    m_pCacheWriteOffset->Color = m_DiffuseColor;
    m_pCacheWriteOffset->TextureCoordinates = m_NeedsBrushTransform ? m_BrushTransform.TransformPoint(point) : Point2F(0.0f, 0.0f);
    m_pCacheWriteOffset->MaskCoordinates = m_NeedsMaskTransform ? m_MaskTransform.TransformPoint(mask) : Point2F(0.0f, 0.0f);

    ++m_pCacheWriteOffset;
}

__checkReturn HRESULT
CGeometryTesselationSink::AddRectangleWithUnitMask(
    const RectF& rect
	)
{
	HRESULT hr = S_OK;

	if (GetAvailableVertexBufferCount() < 6)
	{
		IFC(FlushVertexCache());
	}

    {
        Point2F TopLeft = Point2F(rect.left, rect.top);
        Point2F TopRight = Point2F(rect.right, rect.top);
        Point2F BottomLeft = Point2F(rect.left, rect.bottom);
        Point2F BottomRight = Point2F(rect.right, rect.bottom);  

        Point2F TopLeftMask = Point2F(0.0f, 0.0f);
        Point2F TopRightMask = Point2F(1.0f, 0.0f);
        Point2F BottomLeftMask = Point2F(0.0f, 1.0f);
        Point2F BottomRightMask = Point2F(1.0f, 1.0f);

        AddTrianglePointWithMaskInternal(TopLeft, TopLeftMask);
        AddTrianglePointWithMaskInternal(TopRight, TopRightMask);
        AddTrianglePointWithMaskInternal(BottomLeft, BottomLeftMask);

        AddTrianglePointWithMaskInternal(BottomLeft, BottomLeftMask);
        AddTrianglePointWithMaskInternal(TopRight, TopRightMask);
        AddTrianglePointWithMaskInternal(BottomRight, BottomRightMask);
    }

Cleanup:
	return hr;
}

__checkReturn HRESULT
CGeometryTesselationSink::Flush(
	)
{
	HRESULT hr = S_OK;

	IFC(FlushVertexCache());

Cleanup:	
	return hr;
}

__checkReturn HRESULT
CGeometryTesselationSink::FlushVertexCache(
	)
{
	HRESULT hr = S_OK;
	UINT32 vertexCount = 0;

	vertexCount = GetUsedVertexBufferCount();

	if (vertexCount > 0)
	{
		IVertexBuffer* pBuffer = m_ppVertexBuffers[m_CurrentVertexBuffer];

		IFC(pBuffer->SetVertices(m_pVertexCache, vertexCount));

		IFC(m_pCallback->OnTesselatedGeometryBatch(pBuffer));

		m_CurrentVertexBuffer = (m_CurrentVertexBuffer + 1) % m_VertexBufferCount;		

		m_pCacheWriteOffset = m_pVertexCache;
	}

Cleanup:
	return hr;	
}

size_t
CGeometryTesselationSink::GetAvailableVertexBufferCount(
	)
{
	return m_VertexCacheSize - GetUsedVertexBufferCount();
}

size_t
CGeometryTesselationSink::GetUsedVertexBufferCount(
	)
{
	return (m_pCacheWriteOffset - m_pVertexCache);
}

__checkReturn HRESULT
CGeometryTesselationSink::SetDiffuseColor(
    const ColorF& Color
    )
{
    HRESULT hr = S_OK;
    
    m_DiffuseColor = Color;
    
    return hr;
}

const ColorF&
CGeometryTesselationSink::GetDiffuseColor( 
    )
{
    return m_DiffuseColor;
}

__checkReturn HRESULT
CGeometryTesselationSink::SetTransform(
    const Matrix3X2F& Transform
    )
{
    HRESULT hr = S_OK;
    
    if (m_HasLocalTransform)
    {
        m_Transform = m_LocalTransform * Transform;
    }
    else
    {
        m_Transform = Transform;
    }
    
    return hr;
}

__checkReturn HRESULT
CGeometryTesselationSink::SetBrushTransform(
    __in_opt const Matrix3X2F* pTransform
    )
{
    HRESULT hr = S_OK;
    
    if (pTransform != NULL)
    {
        m_BrushTransform = *pTransform;
        
        m_NeedsBrushTransform = TRUE;
    }
    else
    {
        m_NeedsBrushTransform = FALSE;
    }

    return hr;
}

__checkReturn HRESULT
CGeometryTesselationSink::SetMaskTransform(
    __in_opt const Matrix3X2F* pTransform
    )
{
    HRESULT hr = S_OK;

    if (pTransform != NULL)
    {
        m_MaskTransform = *pTransform;

        m_NeedsMaskTransform = TRUE;
    }
    else
    {
        m_NeedsMaskTransform = FALSE;
    }

    return hr;
}