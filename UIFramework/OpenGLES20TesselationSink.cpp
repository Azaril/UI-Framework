#include "OpenGLES20TesselationSink.h"

COpenGLES20TesselationSink::COpenGLES20TesselationSink(
	)
	: m_ppVertexBuffers(NULL)
	, m_VertexBufferCount(0)
	, m_CurrentVertexBuffer(0)
    , m_Transform(Matrix3X2F::Identity())
    , m_BrushTransform(Matrix3X2F::Identity())
    , m_NeedsBrushTransform(FALSE)
{
	m_pCacheWriteOffset = m_VertexCache;
}

COpenGLES20TesselationSink::~COpenGLES20TesselationSink(
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
}

__checkReturn HRESULT
COpenGLES20TesselationSink::Initialize(
    __in ITesselationBatchCallback* pCallback,
    __in_ecount(VertexBufferCount) COpenGLES20VertexBuffer** ppVertexBuffers,
    UINT32 VertexBufferCount
	)
{
	HRESULT hr = S_OK;

	m_pCallback = pCallback;

	m_ppVertexBuffers = new COpenGLES20VertexBuffer*[VertexBufferCount];
	IFCOOM(m_ppVertexBuffers);

	m_VertexBufferCount = VertexBufferCount;

	for (UINT32 i = 0; i < VertexBufferCount; ++i)
	{
		m_ppVertexBuffers[i] = ppVertexBuffers[i];
		AddRefObject(m_ppVertexBuffers[i]);
	}

Cleanup:
	return hr;
}

__checkReturn HRESULT
COpenGLES20TesselationSink::AddTriangle(
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

	m_pCacheWriteOffset->Position = m_Transform.TransformPoint(point1);
    m_pCacheWriteOffset->Color = m_DiffuseColor;
    m_pCacheWriteOffset->TextureCoordinates = m_NeedsBrushTransform ? m_BrushTransform.TransformPoint(point1) : Point2F(0.0f, 0.0f);

	++m_pCacheWriteOffset;

	m_pCacheWriteOffset->Position = m_Transform.TransformPoint(point2);
    m_pCacheWriteOffset->Color = m_DiffuseColor;
    m_pCacheWriteOffset->TextureCoordinates = m_NeedsBrushTransform ? m_BrushTransform.TransformPoint(point2) : Point2F(0.0f, 0.0f);

	++m_pCacheWriteOffset;

	m_pCacheWriteOffset->Position = m_Transform.TransformPoint(point3);
    m_pCacheWriteOffset->Color = m_DiffuseColor;
    m_pCacheWriteOffset->TextureCoordinates = m_NeedsBrushTransform ? m_BrushTransform.TransformPoint(point3) : Point2F(0.0f, 0.0f);

	++m_pCacheWriteOffset;

Cleanup:
	return hr;
}

__checkReturn HRESULT
COpenGLES20TesselationSink::Flush(
	)
{
	HRESULT hr = S_OK;

	IFC(FlushVertexCache());

Cleanup:	
	return hr;
}

__checkReturn HRESULT
COpenGLES20TesselationSink::FlushVertexCache(
	)
{
	HRESULT hr = S_OK;
	UINT32 vertexCount = 0;

	vertexCount = GetUsedVertexBufferCount();

	if (vertexCount > 0)
	{
		COpenGLES20VertexBuffer* pBuffer = m_ppVertexBuffers[m_CurrentVertexBuffer];

		IFC(pBuffer->SetVertices(m_VertexCache, vertexCount));

		IFC(m_pCallback->OnTesselatedGeometryBatch(pBuffer));

		m_CurrentVertexBuffer = (m_CurrentVertexBuffer + 1) % m_VertexBufferCount;		

		m_pCacheWriteOffset = m_VertexCache;
	}

Cleanup:
	return hr;	
}

size_t
COpenGLES20TesselationSink::GetAvailableVertexBufferCount(
	)
{
	return ARRAYSIZE(m_VertexCache) - GetUsedVertexBufferCount();
}

size_t
COpenGLES20TesselationSink::GetUsedVertexBufferCount(
	)
{
	return (m_pCacheWriteOffset - m_VertexCache);
}

__checkReturn HRESULT
COpenGLES20TesselationSink::SetDiffuseColor(
    const ColorF& Color
    )
{
    HRESULT hr = S_OK;
    
    m_DiffuseColor = Color;
    
    return hr;
}

__checkReturn HRESULT
COpenGLES20TesselationSink::SetTransform(
    const Matrix3X2F& Transform
    )
{
    HRESULT hr = S_OK;
    
    m_Transform = Transform;
    
    return hr;
}

__checkReturn HRESULT
COpenGLES20TesselationSink::SetBrushTransform(
    __in_opt const Matrix3X2F* pTransform
    )
{
    HRESULT hr = S_OK;
    
    if (pTransform != NULL)
    {
        m_BrushTransform = *pTransform;
        
        //TODO: Validate brush transform mapping is correct.
        if (m_BrushTransform.Invert())
        {
            m_NeedsBrushTransform = TRUE;
        }
        else
        {
            m_NeedsBrushTransform = FALSE;
        }
    }
    else
    {
        m_NeedsBrushTransform = FALSE;
    }
     
Cleanup:
    return hr;
}