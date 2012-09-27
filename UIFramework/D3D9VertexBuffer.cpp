#include "D3D9VertexBuffer.h"

#if defined(FRAMEWORK_D3D9)

CD3D9VertexBuffer::CD3D9VertexBuffer(
    )
    : m_pVertexBuffer(NULL)
    , m_MaximumVertexCount(0)
    , m_StoredVertices(0)
{
}

CD3D9VertexBuffer::~CD3D9VertexBuffer(
    )
{
    ReleaseObject(m_pVertexBuffer);
}

__checkReturn HRESULT
CD3D9VertexBuffer::Initialize(
    __in IDirect3DVertexBuffer9* pVertexBuffer
    )
{
    HRESULT hr = S_OK;
    D3DVERTEXBUFFER_DESC vertexBufferDescription = { };

    SetObject(m_pVertexBuffer, pVertexBuffer);

    IFC(pVertexBuffer->GetDesc(&vertexBufferDescription));

    m_MaximumVertexCount = vertexBufferDescription.Size / sizeof(RenderVertex);

Cleanup:
    return hr;
}

__out IDirect3DVertexBuffer9*
CD3D9VertexBuffer::GetD3DVertexBuffer( 
    )
{
    return m_pVertexBuffer;
}

__override __checkReturn HRESULT 
CD3D9VertexBuffer::SetVertices(
    __in_ecount(VertexCount) RenderVertex* pVertices,
    UINT32 VertexCount            
    )
{
    HRESULT hr = S_OK;
    void* pData = NULL;

    UINT32 copySize = VertexCount * sizeof(RenderVertex);

#if defined(_XBOX)
	IFC(m_pVertexBuffer->Lock(0, copySize, &pData, 0));
#else
    IFC(m_pVertexBuffer->Lock(0, copySize, &pData, D3DLOCK_DISCARD));
#endif

    memcpy(pData, pVertices, copySize);

    IFC(m_pVertexBuffer->Unlock());

    m_StoredVertices = VertexCount;

Cleanup:
    return hr;
}

__override UINT32 
CD3D9VertexBuffer::GetVertexCount(
    )
{
    return m_StoredVertices;
}

__override UINT32 
CD3D9VertexBuffer::GetMaximumVertices(
    )
{
    return m_MaximumVertexCount;
}

#endif