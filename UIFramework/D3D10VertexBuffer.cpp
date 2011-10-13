#include "D3D10VertexBuffer.h"

CD3D10VertexBuffer::CD3D10VertexBuffer(
    )
    : m_pBuffer(NULL)
    , m_MaximumVertexCount(0)
    , m_StoredVertices(0)
{
}

CD3D10VertexBuffer::~CD3D10VertexBuffer(
    )
{
    ReleaseObject(m_pBuffer);
}

__checkReturn HRESULT
CD3D10VertexBuffer::Initialize(
    __in ID3D10Buffer* pVertexBuffer
    )
{
    HRESULT hr = S_OK;
    D3D10_BUFFER_DESC bufferDescription = { };

    SetObject(m_pBuffer, pVertexBuffer);

    pVertexBuffer->GetDesc(&bufferDescription);

    m_MaximumVertexCount = bufferDescription.ByteWidth / sizeof(RenderVertex);

    return hr;
}

__out ID3D10Buffer*
CD3D10VertexBuffer::GetD3DBuffer( 
    )
{
    return m_pBuffer;
}

__override __checkReturn HRESULT 
CD3D10VertexBuffer::SetVertices(
    __in_ecount(VertexCount) RenderVertex* pVertices,
    UINT32 VertexCount            
    )
{
    HRESULT hr = S_OK;
    void* pData = NULL;

    UINT32 copySize = VertexCount * sizeof(RenderVertex);

    IFC(m_pBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, &pData));

    memcpy(pData, pVertices, copySize);

    m_StoredVertices = VertexCount;

Cleanup:
    if (pData != NULL)
    {
        m_pBuffer->Unmap();
    }

    return hr;
}

__override UINT32 
CD3D10VertexBuffer::GetVertexCount(
    )
{
    return m_StoredVertices;
}

__override UINT32 
CD3D10VertexBuffer::GetMaximumVertices(
    )
{
    return m_MaximumVertexCount;
}