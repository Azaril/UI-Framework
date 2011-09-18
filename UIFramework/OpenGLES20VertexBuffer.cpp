#include "OpenGLES20VertexBuffer.h"

COpenGLES20VertexBuffer::COpenGLES20VertexBuffer(
	)
	: m_BufferID(0)
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