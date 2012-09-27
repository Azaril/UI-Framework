#include "D3D11VertexBuffer.h"

#if defined(FRAMEWORK_D3D11)

CD3D11VertexBuffer::CD3D11VertexBuffer(
    )
    : m_pBuffer(NULL)
    , m_pContext(NULL)
    , m_MaximumVertexCount(0)
    , m_StoredVertices(0)
{
}

CD3D11VertexBuffer::~CD3D11VertexBuffer(
    )
{
    ReleaseObject(m_pContext);
    ReleaseObject(m_pBuffer);
}

__checkReturn HRESULT
CD3D11VertexBuffer::Initialize(
    __in ID3D11DeviceContext* pContext,
    __in ID3D11Buffer* pVertexBuffer
    )
{
    HRESULT hr = S_OK;
    D3D11_BUFFER_DESC bufferDescription = { };

    SetObject(m_pContext, pContext);
    SetObject(m_pBuffer, pVertexBuffer);

    pVertexBuffer->GetDesc(&bufferDescription);

    m_MaximumVertexCount = bufferDescription.ByteWidth / sizeof(RenderVertex);

    return hr;
}

__out ID3D11Buffer*
CD3D11VertexBuffer::GetD3DBuffer( 
    )
{
    return m_pBuffer;
}

__override __checkReturn HRESULT 
CD3D11VertexBuffer::SetVertices(
    __in_ecount(VertexCount) RenderVertex* pVertices,
    UINT32 VertexCount            
    )
{
    HRESULT hr = S_OK;
    D3D11_MAPPED_SUBRESOURCE mappedBuffer = { };

    UINT32 copySize = VertexCount * sizeof(RenderVertex);

    IFC(m_pContext->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &mappedBuffer));

    memcpy(mappedBuffer.pData, pVertices, copySize);

    m_StoredVertices = VertexCount;

Cleanup:
    if (mappedBuffer.pData != NULL)
    {
        m_pContext->Unmap(m_pBuffer, 0);
    }

    return hr;
}

__override UINT32 
CD3D11VertexBuffer::GetVertexCount(
    )
{
    return m_StoredVertices;
}

__override UINT32 
CD3D11VertexBuffer::GetMaximumVertices(
    )
{
    return m_MaximumVertexCount;
}

#endif