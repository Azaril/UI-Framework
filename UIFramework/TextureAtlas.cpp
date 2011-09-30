#include "TextureAtlas.h"

CTextureAtlas::CTextureAtlas(
	)
	: m_pTexture(NULL)
{
}

CTextureAtlas::~CTextureAtlas(
	)
{
	ReleaseObject(m_pTexture);
}

__checkReturn HRESULT
CTextureAtlas::Initialize(
	__in ITexture* pTexture
	)
{
	HRESULT hr = S_OK;

	m_pTexture = pTexture;
	AddRefObject(m_pTexture);

	return hr;
}

__override __checkReturn HRESULT
CTextureAtlas::AllocateTexture(
	UINT32 Width,
	UINT32 Height,
	__deref_out ITexture** ppTexture
	)
{
	HRESULT hr = S_OK;

	return hr;
}