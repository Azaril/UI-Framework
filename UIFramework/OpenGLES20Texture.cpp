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

GLuint
COpenGLES20Texture::GetTextureID(
    )
{
    return m_TextureID;
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

__override __checkReturn HRESULT 
COpenGLES20Texture::SetData(
    __in_ecount(DataSize) BYTE* pData,
    UINT32 DataSize,
    INT32 Stride
    )
{
    HRESULT hr = S_OK;
    
    //TODO: Handle stride and data size to enforce the bits can be copied in to opengl.
    
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData);
    
    return hr;
}

__override __checkReturn HRESULT 
COpenGLES20Texture::SetSubData(
    const RectU& Region,
    __in_ecount(DataSize) BYTE* pData,
    UINT32 DataSize,
    INT32 Stride
    )
{
    HRESULT hr = S_OK;
    
    //TODO: Handle stride and data size to enforce the bits can be copied in to opengl.
    
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    
    glTexSubImage2D(GL_TEXTURE_2D, 0, Region.left, Region.top, Region.right - Region.left, Region.bottom - Region.top, GL_RGBA, GL_UNSIGNED_BYTE, pData);
    
    return hr;
}

__override __checkReturn HRESULT 
COpenGLES20Texture::SetMultipleSubData(
    __in_ecount(RegionCount) const RectU* pRegions,
    __in_ecount(RegionCount) BYTE** ppData,
    __in_ecount(RegionCount) UINT32* pDataSizes,
    __in_ecount(RegionCount) INT32* pStrides,
    UINT32 RegionCount
    )
{
    HRESULT hr = S_OK;
    
    //TODO: Is this optimal, is one lock better?
    for (UINT32 i = 0; i < RegionCount; ++i)
    {
        IFC(SetSubData(pRegions[i], ppData[i], pDataSizes[i], pStrides[i]));
    }
    
Cleanup:
    return hr;
}