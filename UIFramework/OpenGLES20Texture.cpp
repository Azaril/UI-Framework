#include "OpenGLES20Texture.h"

COpenGLES20Texture::COpenGLES20Texture(
	)
	: m_TextureID(0)
	, m_Width(0)
	, m_Height(0)
	, m_PixelFormat(PixelFormat::Unknown)
{
}

COpenGLES20Texture::~COpenGLES20Texture(
	)
{
	//TODO: Assert the correct context is being used here...

	if (m_TextureID != 0)
	{
		glDeleteTextures(1, &m_TextureID);
	}
}

__checkReturn HRESULT
COpenGLES20Texture::Initialize(
	GLuint TextureID,
	UINT32 Width,
	UINT32 Height,
	PixelFormat::Value Format
	)
{
	HRESULT hr = S_OK;

	m_TextureID = TextureID;
	m_Width = Width;
	m_Height = Height;
	m_PixelFormat = Format;

	return hr;
}

__override UINT32 
COpenGLES20Texture::GetWidth(
    )
{
	return m_Width;
}

__override UINT32 
COpenGLES20Texture::GetHeight(
    )
{
	return m_Height;
}

__override INT32 
COpenGLES20Texture::GetStride(
    )
{
	//TODO: Base this on pixel format.
	return m_Width * (sizeof(BYTE) * 4);
}

__override PixelFormat::Value 
COpenGLES20Texture::GetPixelFormat(
    )
{
	return m_PixelFormat;
}