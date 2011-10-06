#include "OpenGLES20TextureAtlas.h"

COpenGLES20TextureAtlas::COpenGLES20TextureAtlas(
	)
	: m_pWhitePixel(NULL)
{
	
}

COpenGLES20TextureAtlas::~COpenGLES20TextureAtlas(
	)
{
	ReleaseObject(m_pWhitePixel);
}

__checkReturn HRESULT
COpenGLES20TextureAtlas::Initialize(
	__in ITexture* pTexture
	)
{
	HRESULT hr = S_OK;

	IFC(CTextureAtlas< 1 >::Initialize(pTexture));

	IFC(AllocateTexture(1, 1, &m_pWhitePixel));

	switch (m_pWhitePixel->GetPixelFormat())
	{
		case PixelFormat::B8G8R8A8:
			{
				BYTE whitePixel[] = { 0xFF, 0xFF, 0xFF, 0xFF };

				IFC(m_pWhitePixel->SetData(whitePixel, sizeof(whitePixel), PixelFormat::GetLineSize(PixelFormat::B8G8R8A8, 1)));

				break;
			}

		default:
			{
				IFC(E_UNEXPECTED);
			}
	}

Cleanup:
	return hr;
}

__out ITexture*
COpenGLES20TextureAtlas::GetWhitePixelTexture(
    )
{
    return m_pWhitePixel;
}