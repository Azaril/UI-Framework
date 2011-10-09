#include "OpenGLES20VertexBuffer.h"

COpenGLES20VertexBuffer::COpenGLES20VertexBuffer(
	)
	: m_BufferID(0)
    , m_VertexCount(0)
{
}

COpenGLES20VertexBuffer::~COpenGLES20VertexBuffer(
	)
{
	if (m_BufferID != 0)
	{
		glDeleteBuffers(1, &m_BufferID);
	}
}

__checkReturn HRESULT
COpenGLES20VertexBuffer::Initialize(
	GLuint BufferID
	)
{
	HRESULT hr = S_OK;

	m_BufferID = BufferID;

	return hr;
}

__checkReturn HRESULT
COpenGLES20VertexBuffer::SetVertices(
    __in_ecount(VertexCount) RenderVertex* pVertices,
    UINT32 VertexCount     
    )
{
    HRESULT hr = S_OK;
    
    glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(RenderVertex) * VertexCount, pVertices, GL_STREAM_DRAW);
    
    m_VertexCount = VertexCount;
    
    return hr;
}

__checkReturn HRESULT
COpenGLES20VertexBuffer::Bind(
    GLuint Buffer
    )
{
    HRESULT hr = S_OK;
    
    glBindBuffer(Buffer, m_BufferID);
    
    return hr;
}

UINT32
COpenGLES20VertexBuffer::GetVertexCount(
    )
{
    return m_VertexCount;
}

UINT32 
COpenGLES20VertexBuffer::GetMaximumVertices(
    )
{
    return UINT32_MAX;
}